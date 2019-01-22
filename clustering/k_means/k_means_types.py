#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""K-means clustering types and utils functions."""

from typing import *

T = TypeVar('T')

Point = Tuple[float, float]
Cluster = List[Point]
Centroid = Point
Centroids = List[Centroid]
Clusters = List[Cluster]
Circle = Tuple[Point, float]
MaybeCircles = Union[List[Circle], None]


def concat(list_of_lists: Iterable[Iterable[T]]) -> List[T]:
    """Flatten the specified list of lists."""
    return sum(list_of_lists, [])


def main():
    """Launcher."""
    pass


if __name__ == "__main__":
    main()
