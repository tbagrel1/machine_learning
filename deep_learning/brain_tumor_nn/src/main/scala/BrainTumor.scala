import java.io.File

import org.deeplearning4j.optimize.listeners.ScoreIterationListener
import org.deeplearning4j.util.ModelSerializer
import org.nd4j.evaluation.classification.Evaluation

import Logger._
import Model._
import Params._
import Env._
import Data._

object  BrainTumor {
  def main(args: Array[String]): Unit = {
    val startWith = args(0).toInt
    val epochsNb = args(1).toInt
    val currentModel = if (startWith == 0) {
      model
    } else {
      ModelSerializer.restoreMultiLayerNetwork(new File(modelSavePath, s"after_epoch_${startWith}.nnsave"))
    }

    log.info("Train model...")
    currentModel.setListeners(new ScoreIterationListener(1))
    for (i <- startWith + 1 to startWith + epochsNb) {
      log.info(s"    Doing epoch ${i}...")
      currentModel.fit(trainDatasetIterator)
      log.info(s"    Completed epoch ${i}!")

      log.info("    Evaluate model...")
      val eval = new Evaluation(classesNb)

      while (testDatasetIterator.hasNext) {
        val ds = testDatasetIterator.next()
        val output = currentModel.output(ds.getFeatures(), false)
        eval.eval(ds.getLabels, output)
      }

      log.info(eval.stats())
      log.info("    Model evaluated!")

      testDatasetIterator.reset()

      ModelSerializer.writeModel(currentModel, new File(modelSavePath, s"after_epoch_${i}.nnsave"), true)
    }
    log.info("Model trained!")
    log.info("***** END *****")

  }
}
