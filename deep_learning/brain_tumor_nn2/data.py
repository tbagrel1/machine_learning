from tensorflow.keras.preprocessing.image import img_to_array
from tensorflow.keras.utils import to_categorical, Sequence
import cv2
import os
import numpy as np
import random
import math

from params import *

labels_map = {
    "a": 3,
    "b": 2,
    "c": 1,
    "z": 0
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

def get_data_labels(dataset_path, limit=None):
    data = []
    labels = []

    if limit is None:
        limit = EPOCH_MAX_SIZE

    image_paths = sorted(list(list_images(dataset_path)))
    # print(image_paths)
    random.shuffle(image_paths)
    image_paths = image_paths[:min(len(image_paths), limit)]
    for apath in image_paths:
        image = cv2.imread(apath, cv2.IMREAD_GRAYSCALE)
        image = img_to_array(image)
        data.append(image)

        label = apath.split(os.path.sep)[-2]
        label = labels_map[label]
        labels.append(label)

    print("{} {}".format(len(data), len(labels)))

    data = np.array(data, dtype="float") / 255.0
    data = data.reshape(data.shape[0], WIDTH, HEIGHT, 1)
    labels = np.array(labels)
    labels = to_categorical(labels, num_classes=CLASSES_NB)

    for i, (apath, label) in enumerate(zip(image_paths, labels)):
        print("{} === {}".format(apath, label))
        if i > 1000:
            break

    return (data, labels)


class DataLabelsGenerator(Sequence):
    def __init__(self, dataset_path, batch_size):
        self.image_paths = sorted(list(list_images(dataset_path)))
        random.shuffle(self.image_paths)
        self.batch_size = batch_size

    def __len__(self):
        return math.ceil(len(self.image_paths) / self.batch_size)

    def __getitem__(self, idx):
        batch_image_paths = self.image_paths[idx * self.batch_size:(idx + 1) * self.batch_size]
        images = [img_to_array(cv2.imread(apath, cv2.IMREAD_GRAYSCALE)) for apath in batch_image_paths]
        images = np.array(images, dtype="float") / 255.0
        images = images.reshape(images.shape[0], WIDTH, HEIGHT, CHANNELS)
        labels = [labels_map[apath.split(os.path.sep)[-2]] for apath in batch_image_paths]
        labels = np.array(labels)
        labels = to_categorical(labels, num_classes=CLASSES_NB)
        # print("\n\n")
        #
        # for (apath, label) in zip(batch_image_paths, labels):
        #     print("{} === {}".format(apath, label))
        #
        # print("\n\n")

        return images, labels
