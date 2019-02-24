import collection.JavaConverters._

object Params {
  val trainDataPercent = 90
  val testDataPercent = 10

  val batchSize = 64

  val seed = System.currentTimeMillis().toInt

  val l2Value = 0.0005
  val momentum = 1.0

  val learningRateSchedule = List(
    0 -> 0.06,
    200 -> 0.05,
    600 -> 0.028,
    800 -> 0.0060,
    1000 -> 0.001
  ).map { case (k, v) => (Int.box(k), Double.box(v)) }.toMap.asJava
}
