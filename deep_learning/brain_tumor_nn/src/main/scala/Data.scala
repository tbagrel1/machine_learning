import java.io.File

import org.datavec.api.split.FileSplit
import org.datavec.image.loader.BaseImageLoader
import java.util.Random

import org.datavec.api.io.filters.BalancedPathFilter
import org.datavec.api.io.labels.ParentPathLabelGenerator
import org.datavec.image.recordreader.ImageRecordReader
import org.deeplearning4j.datasets.datavec.RecordReaderDataSetIterator
import org.nd4j.linalg.dataset.api.preprocessor.ImagePreProcessingScaler

object Data {
  import Env._
  import Params._
  import Logger._

  val randNumGen = new Random(seed)
  val baseDir = new File(datasetPath)
  val filesInDir = new FileSplit(baseDir, allowedFormats, randNumGen)
  val labelMaker = new ParentPathLabelGenerator()
  val pathFilter = new BalancedPathFilter(randNumGen, allowedFormats, labelMaker)

  log.info("Load data...")

  val filesInDirSplit = filesInDir.sample(pathFilter, trainDataPercent, testDataPercent)

  log.info("    Load train data...")
  val trainData = filesInDirSplit(0)
  val trainRecordReader = new ImageRecordReader(height, width, channelsNb, labelMaker)
  trainRecordReader.initialize(trainData)
  val trainDatasetIterator = new RecordReaderDataSetIterator(trainRecordReader, batchSize, 1, classesNb)
  val trainScaler = new ImagePreProcessingScaler(0, 1)
  trainScaler.fit(trainDatasetIterator)
  trainDatasetIterator.setPreProcessor(trainScaler)
  log.info("    Train data loaded!")

  log.info("    Load test data...")
  val testData = filesInDirSplit(1)
  val testRecordReader = new ImageRecordReader(height, width, channelsNb, labelMaker)
  testRecordReader.initialize(testData)
  val testDatasetIterator = new RecordReaderDataSetIterator(testRecordReader, batchSize, 1, classesNb)
  val testScaler = new ImagePreProcessingScaler(0, 1)
  testScaler.fit(testDatasetIterator)
  testDatasetIterator.setPreProcessor(testScaler)
  log.info("    Test data loaded!")

  log.info("Data loaded!")
}
