from tensorflow.keras.utils import Sequence
import os
import numpy as np
import random
import math
import binimage

from params import *

labels_map = {
    "a": np.array([0., 0., 0., 1.]),
    "b": np.array([0., 0., 1., 0.]),
    "c": np.array([0., 1., 0., 0.]),
    "z": np.array([1., 0., 0., 0.])
}

image_types = (".pgm")

# random.seed(31)

def list_images(basePath, contains=None):
    # return the set of files that are valid
    return list_files(basePath, validExts=image_types, contains=contains)


def list_files(basePath, validExts=None, contains=None):
    # loop over the directory structure
    for (rootDir, dirNames, filenames) in os.walk(basePath):
        # loop over the filenames in the current directory
        for filename in filenames:
            # if the contains string is not none and the filename does not contain
            # the supplied string, then ignore the file
            if contains is not None and filename.find(contains) == -1:
                continue

            # determine the file extension of the current file
            ext = filename[filename.rfind("."):].lower()

            # check to see if the file is an image and should be processed
            if validExts is None or ext.endswith(validExts):
                # construct the path to the image and yield it
                imagePath = os.path.join(rootDir, filename)
                yield imagePath


class DataLabelsGenerator(Sequence):
    def __init__(self, dataset_path, batch_size):
        self.image_paths = sorted(list(list_images(dataset_path)))
        random.shuffle(self.image_paths)
        self.batch_size = batch_size

    def __len__(self):
        return math.ceil(len(self.image_paths) / self.batch_size)

    def __getitem__(self, index):
        batch_image_paths = self.image_paths[
            index * self.batch_size:(index + 1) * self.batch_size]
        return (np.array([
            binimage.load_binpgm(apath.encode("utf-8"))
            for apath in batch_image_paths
        ]), np.array([
            labels_map[apath.split(os.path.sep)[-2]]
            for apath in batch_image_paths
        ]))
