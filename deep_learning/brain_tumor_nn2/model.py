# -*- coding: utf-8 -*-

import tensorflow as tf
from tensorflow.keras import *
from params import *

def get_model():
    model = Sequential()
    model.add(layers.Conv2D(
        32, (3, 3), strides=(1, 1),
        activation="relu", input_shape=INPUT_SHAPE,
        kernel_regularizer=regularizers.l2(L2_COEFF),
        bias_regularizer=regularizers.l2(L2_COEFF)
    ))
    model.add(layers.MaxPool2D(
        pool_size=(2, 2), strides=(2, 2)
    ))
    model.add(layers.Conv2D(
        64, (3, 3), strides=(1, 1), activation="relu",
        kernel_regularizer=regularizers.l2(L2_COEFF),
        bias_regularizer=regularizers.l2(L2_COEFF)
    ))
    model.add(layers.MaxPool2D(
        pool_size=(2, 2), strides=(2, 2)
    ))
    model.add(layers.Conv2D(
        128, (3, 3), strides=(1, 1), activation="relu",
        kernel_regularizer=regularizers.l2(L2_COEFF),
        bias_regularizer=regularizers.l2(L2_COEFF)
    ))
    model.add(layers.MaxPool2D(
        pool_size=(2, 2), strides=(2, 2)
    ))
    model.add(layers.Conv2D(
        256, (3, 3), strides=(1, 1), activation="relu",
        kernel_regularizer=regularizers.l2(L2_COEFF),
        bias_regularizer=regularizers.l2(L2_COEFF)
    ))
    model.add(layers.MaxPool2D(
        pool_size=(2, 2), strides=(2, 2)
    ))
    # model.add(layers.Conv2D(
    #     256, (5, 5), strides=(1, 1), activation="relu"
    # ))
    # model.add(layers.MaxPool2D(
    #     pool_size=(2, 2), strides=(2, 2)
    # ))
    # model.add(layers.Conv2D(
    #     512, (5, 5), strides=(1, 1), activation="relu"
    # ))
    # model.add(layers.MaxPool2D(
    #     pool_size=(2, 2), strides=(2, 2)
    # ))
    model.add(layers.Flatten())
    model.add(layers.Dense(
        1024, activation="relu",
        kernel_regularizer=regularizers.l2(L2_COEFF),
        bias_regularizer=regularizers.l2(L2_COEFF)
    ))
    model.add(layers.Dense(
        CLASSES_NB, activation="softmax"
    ))
    return model
