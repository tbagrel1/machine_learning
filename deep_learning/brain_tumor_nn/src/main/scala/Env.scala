import org.datavec.image.loader.BaseImageLoader

object Env {
  val datasetPath: String = "data/20190211111120/"
  val modelSavePath: String = "nn_saves/cnn_6/"
  val allowedFormats: Array[String] = Array("pgm")
  val height = 128
  val width = 96
  val channelsNb = 1
  val classesNb = 4
}
