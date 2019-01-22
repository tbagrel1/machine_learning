#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""K-means clustering with points on a 2D surface."""

import random
import math
import operator

from k_means_plot import *
from k_means_types import *

SECOND_ITEM = operator.itemgetter(1)
INVALID = -1


def dist(point_a: Point, point_b: Point) -> float:
    """Retourne la distance entre les deux points."""
    return ((point_b[0] - point_a[0]) ** 2 +
            (point_b[1] - point_a[1]) ** 2) ** 0.5


def stability_measure(
        prev_points: List[Point], new_points: List[Point]) -> float:
    """Retourne une évaluation de la stabilité des points spécifiés, basée
    sur leur mouvement par rapport à leur position précédente."""
    return max((dist(prev_point, new_point)
                for prev_point, new_point in zip(prev_points, new_points)))


def homogeneity_perf(centroids: Centroids, clusters: Clusters) -> float:
    """Retourne une évalutation de l'homogénéité en taille des clusters.
    Higher is better."""
    lengths = [len(cluster) for cluster in clusters]
    log_lengths = [math.log(length, 10) + 1 for length in lengths]
    return 1 / (1 + std_dev(log_lengths) / max(log_lengths))


def proximity_perf(centroids: Centroids, clusters: Clusters) -> float:
    """Retourne une évaluation de la proximité des points d'un cluster à leur
    centroid. Higher is better."""
    proximity_index = max((
        mean([
            dist(point, centroid)
            for point in cluster
        ])
        for (centroid, cluster) in zip(centroids, clusters)
        if len(cluster) > 1
    ))
    return 1 / (1 + proximity_index)


def distinction_perf(centroids: Centroids, clusters: Clusters) -> float:
    """Evalue si les clusters formés sont distinguables facilement.
    Higher is better."""
    return math.atan(10 * min((
        dist(centroids[i], centroids[j])
        for i in range(len(centroids))
        for j in range(i))
    ))


def rand_point(bottom_left: Point, top_right: Point) -> Point:
    """Retourne un point aléatoire dans le cadre défini par bottom_left et
    top_right."""
    return (
        random.random() * (top_right[0] - bottom_left[0]),
        random.random() * (top_right[1] - bottom_left[1]))


def copy_centroids(centroids: List[Point]) -> List[Point]:
    """Retourne une deep copy de la liste de centroids spécifiée."""
    return [centroid[:] for centroid in centroids]


def mean(data: List[float]) -> float:
    """Retourne la moyenne de la série statistique spécifiée."""
    return sum(data) / len(data)


def mean_points(points: List[Point]) -> Point:
    """Retourne le point moyen de l'ensemble de points spécifiés."""
    return (mean([point[0] for point in points]),
            mean([point[1] for point in points]))


def k_mean_clustering(
        k: int, points: List[Point], tries: int,
        bottom_left: Point, top_right: Point) -> \
        Tuple[List[Centroids], List[Clusters], float]:
    """Réalise un k-clustering de l'ensemble de points spécifiés.
    Paramètres :
        - k : le nombre de clusters désirés
        - points : la liste de points à partitionner
        - tries : nombre d'itérations de l'algorithme
        - bottom_left : premier point du cadre de travail
        - top_right : second point du cadre de travail
    Retourne :
        - centroidss : centroids de chacune des itérations
        - clusterss : clusters de chacune des itérations
        - current_eps : mesure de stabilité obtenue en sortie des itérations
    """
    centroids = [rand_point(bottom_left, top_right) for _ in range(k)]
    centroidss = [copy_centroids(centroids)]
    clusterss = []
    current_try = 1
    current_eps = -1
    while current_try <= tries:
        clusters = [[] for _ in range(k)]
        for point in points:
            # Trouve le centroid duquel le point est le plus proche
            its_centroid_index = min((
                (i, dist(point, centroid))
                for i, centroid in enumerate(centroids)), key=SECOND_ITEM)[0]
            clusters[its_centroid_index].append(point)
        centroids = [
            prev_centroid if len(cluster) == 0 else mean_points(cluster)
            for prev_centroid, cluster in zip(centroids, clusters)]
        centroidss.append(centroids)
        clusterss.append(clusters)
        current_try += 1
        current_eps = stability_measure(centroidss[-2], centroids)
    return centroidss, clusterss, current_eps


def points_around(n: int, center: Point, max_radius: float) -> List[Point]:
    """Retourne un liste de n points aléatoires centrés sur center avec un
    rayon maximum max_radius."""
    result = []
    for _ in range(n):
        radius = random.random() * max_radius
        angle = random.random() * 2 * math.pi
        result.append((
            center[0] + radius * math.cos(angle),
            center[1] + radius * math.sin(angle)))
    return result


def std_dev(values: List[float]) -> float:
    """Retourne l'écart type des valeurs spécifiées."""
    m = mean(values)
    return mean([(v - m) ** 2 for v in values]) ** 0.5


def perf_measure(
        last_centroids: Centroids, last_clusters: Clusters,
        final_eps: float) -> Tuple[float, List[float]]:
    lengths = list(map(len, last_clusters))
    """Retourne une évaluation d'un clustering sous forme d'un score final,
    et d'une liste des indicateurs intermédiaires. Higher is better."""

    # Si un des clusters est vide, alors on n'a pas réalisé un k-mean
    # clustering, mais au mieux un (k-1)-mean clustering. On invalide donc
    # l'échantillon
    if 0 in lengths:
        return INVALID, [INVALID, INVALID, INVALID, INVALID]

    h_p = homogeneity_perf(last_centroids, last_clusters)
    p_p = proximity_perf(last_centroids, last_clusters)
    d_p = distinction_perf(last_centroids, last_clusters)
    s_p = 1 / (1 + final_eps)

    overall_perf = h_p * p_p * d_p * s_p

    return (
        overall_perf, [h_p, p_p, d_p, s_p]
    )


def best_k_mean_clustering(
        k_min: int, k_max: int, tries_per_k: int, points: List[Point],
        tries: int, bottom_left: Point, top_right: Point) -> \
        Tuple[int, float, Centroids, Clusters]:
    """Tente de trouver le meilleur clustering pour l'ensemble points.
    Paramètres :
        - k_min : plus petit k-mean clustering à essayer
        - k_max : plus grand k-mean clustering à essayer
        - tries_per_k : nombre d'essais de k-mean clustering pour chaque k
    Retourne :
        - best_k : meilleur nombre de clusters
        - best_perf_measure : indicateur de performance du meilleur clustering
                              trouvé
        - best_centroids : centroids finaux du meilleur clustering
        - best_clusters : clusters finaux du meilleur clustering
    """
    best_k = 0
    best_perf = 0
    best_centroids = []
    best_clusters = []
    for k in range(k_min, k_max + 1):
        for i in range(tries_per_k):
            centroidss, clusterss, final_eps = k_mean_clustering(
                k, points, tries, bottom_left, top_right)
            perf, sub_perfs = perf_measure(
                centroidss[-1], clusterss[-1], final_eps)
            print("k = {} ({}): {} = h({}) * p({}) * d({}) * s({})"
                  .format(k, i + 1, perf, *sub_perfs))
            if perf > best_perf:
                best_perf = perf
                best_k = k
                best_centroids = centroidss[-1]
                best_clusters = clusterss[-1]
    return best_k, best_perf, best_centroids, best_clusters


def demo_values(chosen_subset: List[int]) -> \
        Tuple[Point, Point, Clusters, List[Point], List[Circle]]:
    """Verse aléatoirement des 'sacs de billes' sur une zone de 10x10."""
    bottom_left = (0, 0)
    top_right = (10, 10)
    zones = [
        ((1, 1), 1),
        ((1, 9), 1.5),
        ((9, 1.5), 0.7),
        ((9, 9), 0.8),
        ((3, 4), 2.3),
        ((6, 2), 1.3),
        ((2, 6), 1.7),
        ((8, 3), 0.9)
    ]
    quantities = [
        15,
        10,
        12,
        20,
        20,
        14,
        20,
        8
    ]

    circles = [zones[i] for i in range(len(zones)) if i in chosen_subset]
    right_clusters = [
        points_around(quantities[i], *zones[i])
        for i in range(len(zones)) if i in chosen_subset
    ]
    points = concat(right_clusters)

    return bottom_left, top_right, right_clusters, points, circles


def demo_given(show_circles: bool = False):
    """Example pas à pas de l'algorithme k-mean."""
    tries = 20
    subset = [0, 1, 2, 3, 4, 5]

    k = len(subset)
    bottom_left, top_right, right_clusters, points, circles = \
        demo_values(subset)
    centroidss, clusterss, final_eps = k_mean_clustering(
        k, points, tries, bottom_left, top_right)
    step_by_step_plot(centroidss, clusterss, final_eps, bottom_left, top_right,
                      circles if show_circles else None)


def demo_guess(show_circles: bool = False):
    """Example pas à pas de mon algorithme guess-k-mean."""
    k_min = 2
    k_max = 12
    tries_per_k = 10
    subset = [0, 1, 2, 3, 4, 5]

    bottom_left, top_right, right_clusters, points, circles = \
        demo_values(subset)

    best_k, best_perf_measure, best_centroids, best_clusters = \
        best_k_mean_clustering(k_min, k_max, tries_per_k, points, tries_per_k,
                               bottom_left, top_right)
    print(best_k, best_perf_measure)
    one_plot(best_centroids, best_clusters, bottom_left, top_right,
             circles if show_circles else None)


def main():
    """Lanceur."""
    demo_given(False)


if __name__ == "__main__":
    main()
