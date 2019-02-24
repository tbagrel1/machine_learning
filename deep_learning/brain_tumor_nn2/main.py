# -*- coding: utf-8 -*-

import click
from tensorflow.keras import *
from os import path
import matplotlib.pyplot as plt

from data import DataLabelsGenerator, get_data_labels
from model import get_model
from params import *
from callbacks import *

@click.command()
@click.option("--from-path", type=click.Path(exists=True, file_okay=True, dir_okay=False, readable=True), default=None)
@click.option("--to-path", type=click.Path(), default=DEFAULT_SAVE_PATH)
@click.option("-b", "--save-best-only", is_flag=True, default=False)
@click.option("-n", "--epoch-nb", type=int, default=1)
def main(from_path, to_path, save_best_only, epoch_nb):
    if from_path is None:
        model = get_model()
        model.compile(
            optimizer=optimizers.SGD(
                lr=0.005, decay=1e-6, momentum=0.9, nesterov=True),
            loss=losses.categorical_crossentropy,
            metrics=["accuracy"]
        )
    else:
        print("### LOADED YAY! ###")
        model = models.load_model(from_path)

    history = get_history()
    model_saver = get_model_saver(to_path, save_best_only)
    callbacks = [
        history,
        model_saver
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
        use_multiprocessing=False,
        callbacks=callbacks
    )

    # for i in range(1, epoch_nb + 1):
    #     print("\n%%%%% DOING EPOCH {} %%%%%\n".format(i))
    #     model.fit(
    #         *get_data_labels("data/train_data_hard"),
    #         batch_size=BATCH_SIZE,
    #         epochs=1,
    #         verbose=1,
    #         validation_data=get_data_labels("data/test_data_hard", limit=10000),
    #         callbacks=[model_saver]
    #     )
    print("\n\n--- [END TRAINING]\n\n")

if __name__ == "__main__":
    main()

