# -*- coding: utf-8 -*-

from tensorflow.keras import *

from params import *

class AccuracyHistory(callbacks.Callback):
    def on_train_begin(self, logs={}):
        self.acc = []
    def on_epoch_end(self, epoch, logs={}):
        self.acc.append(logs.get("acc"))

def get_history():
    return AccuracyHistory()

def get_model_saver(to_path, save_best_only):
    return callbacks.ModelCheckpoint(to_path, monitor="val_acc", verbose=1, save_best_only=save_best_only, mode="max")
