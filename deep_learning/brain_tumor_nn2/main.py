# -*- coding: utf-8 -*-

import click
from os import path
import time
import tensorflow as tf
from tensorflow.keras import *
from tensorflow.keras.backend import set_session

from data import DataLabelsGenerator
from model import get_model
from callbacks import *

@click.command()
@click.option("--from-path", type=click.Path(exists=True, file_okay=True, dir_okay=False, readable=True), default=None)
@click.option("--to-path", type=click.Path(), default=DEFAULT_SAVE_PATH)
@click.option("-b", "--save-best-only", is_flag=True, default=False)
@click.option("-n", "--epoch-nb", type=int, default=1)
def main(from_path, to_path, save_best_only, epoch_nb):

    config = tf.ConfigProto()
    config.gpu_options.allow_growth = True
    config.gpu_options.per_process_gpu_memory_fraction = 0.65
    sess = tf.Session(config=config)
    set_session(sess)

    if from_path is None:
        model = get_model()
        model.compile(
            optimizer=optimizers.SGD(
                lr=0.005, decay=1e-6, momentum=0.9, nesterov=True),
            loss=losses.categorical_crossentropy,
            metrics=["accuracy"]
        )
    else:
        model = models.load_model(from_path)

    history = get_history()
    model_saver = get_model_saver(to_path, save_best_only)


    # http://fizzylogic.nl/2017/05/08/monitor-progress-of-your-keras-based-neural-network-using-tensorboard/

    tensorboard = callbacks.TensorBoard(log_dir="logs/{}_{}".format(
        path.splitext(path.basename(path.basename(to_path)))[0],
        time.strftime("%y%m%d_%H%M%S", time.localtime())
    ))
    _callbacks = [
        history,
        model_saver,
        tensorboard
    ]

    train_generator = DataLabelsGenerator("data/train_data_hard", BATCH_SIZE)
    test_generator = DataLabelsGenerator("data/test_data_hard", BATCH_SIZE)

    print("\n\n--- [BEGIN TRAINING]\n\n")
    model.fit_generator(
        train_generator,
        steps_per_epoch=len(train_generator),
        epochs=epoch_nb,
        verbose=1,
        validation_data=test_generator,
        validation_steps=len(test_generator),
        use_multiprocessing=True,
        callbacks=_callbacks
    )
    print("\n\n--- [END TRAINING]\n\n")

if __name__ == "__main__":
    main()

