#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""K-mean clustering plot functions."""

import matplotlib.pyplot as plt
from random import random as rd

from k_means_types import *

COLORS = [
    "red",
    "yellow",
    "lime",
    "orange",
    "cyan",
    "purple",
    "green",
    "magenta",
    "maroon",
    "olive",
    "navy",
    "orangered",
    "saddlebrown",
    "teal",
    "bisque"
] + [
    (rd(), rd(), rd())
    for i in range(100)
]

CIRCLE_COLOR = "thistle"
CIRCLE_ALPHA = 0.3

DEFAULT_POINT_COLOR = "SlateGray"

CENTROID_MARKER = "*"
POINT_MARKER = "."

figure, axes = None, None


def plot_init(bottom_left: Point, top_right: Point):
    """Initializes the plot session, and set globals figure and axes."""
    global figure
    global axes

    plt.ion()
    figure, axes = plt.subplots(1, 1)
    axes.set_xlim(bottom_left[0], top_right[0])
    axes.set_ylim(bottom_left[1], top_right[1])
    axes.set_aspect("equal", adjustable="box")


def plot_finalize():
    """Finalizes the plot session, and reset globals figure and axes."""
    global figure
    global axes

    plot_refresh()
    plt.ioff()
    plt.show()

    figure, axes = None, None


def plot_point(point: Point, refresh: bool = False, **plot_options: Any):
    """Plots the specified point."""
    axes.plot([point[0]], [point[1]], linestyle='None', **plot_options)

    if refresh: plot_refresh()


def plot_points(points: List[Point], refresh: bool = False,
                **plot_options: Any):
    """Plots the specified list of points."""
    axes.plot([point[0] for point in points], [point[1] for point in points],
              linestyle='None', **plot_options)

    if refresh: plot_refresh()


def plot_centroids(centroids: Centroids, refresh: bool = False):
    """Plots the specified list of centroids."""
    for i, centroid in enumerate(centroids):
        plot_point(centroid, False, color=COLORS[i], marker=CENTROID_MARKER)

    if refresh: plot_refresh()


def plot_clusters(clusters: Clusters, refresh: bool = False):
    """Plots the specified list of cluster."""
    for i, cluster in enumerate(clusters):
        plot_points(cluster, False, color=COLORS[i], marker=POINT_MARKER)

    if refresh: plot_refresh()


def plot_circles(circles: List[Circle], refresh: bool = False):
    """Plots the specified list of circles if not None."""
    if circles is not None:
        for center, radius in circles:
            axes.add_artist(plt.Circle(
                center, radius, color=CIRCLE_COLOR, fill=False,
                alpha=CIRCLE_ALPHA))

    if refresh: plot_refresh()


def plot_refresh():
    """Refreshes the plotting area. Needed to see changes after a plot."""
    figure.canvas.draw()


def plot_clear():
    """Clears the plotting area."""
    plt.cla()


def one_plot(centroids: Centroids, clusters: Clusters, bottom_left: Point,
             top_right: Point, circles: MaybeCircles = None):
    """Plots the specified (centroids, clusters) with colors."""

    plot_init(bottom_left, top_right)

    plot_centroids(centroids)
    plot_clusters(clusters)
    plot_circles(circles)

    plot_finalize()


def step_by_step_plot(
        centroidss: List[Centroids], clusterss: List[Clusters], eps: float,
        bottom_left: Point, top_right: Point, circles: MaybeCircles = None):
    """Plots step by step the k-mean algorithm evolution."""

    plot_init(bottom_left, top_right)

    # show the window on screen
    plot_refresh()

    points = concat(clusterss[0])
    plot_points(points, True, color=DEFAULT_POINT_COLOR, marker=POINT_MARKER)

    input("Put centroids ?")
    plot_centroids(centroidss[0])
    plot_circles(circles, True)

    n = len(clusterss)
    for iteration in range(1, n + 1):
        input("Change clusters ? ({})".format(iteration))
        plot_clear()
        plot_centroids(centroidss[iteration - 1])
        plot_clusters(clusterss[iteration - 1])
        plot_circles(circles, True)

        input("Move centroids ? ({})".format(iteration))
        plot_clear()
        plot_centroids(centroidss[iteration])
        plot_clusters(clusterss[iteration - 1])
        plot_circles(circles, True)

    print("\n--> Done with final eps = {}".format(eps))
    plot_finalize()


def main():
    """Launcher."""
    pass


if __name__ == "__main__":
    main()
