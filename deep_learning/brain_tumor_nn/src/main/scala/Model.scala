import org.deeplearning4j.nn.api.OptimizationAlgorithm
import org.deeplearning4j.nn.conf.inputs.InputType
import org.deeplearning4j.nn.conf.layers.{ConvolutionLayer, DenseLayer, OutputLayer, SubsamplingLayer}
import org.deeplearning4j.nn.conf.{MultiLayerConfiguration, NeuralNetConfiguration}
import org.deeplearning4j.nn.multilayer.MultiLayerNetwork
import org.deeplearning4j.nn.weights.WeightInit
import org.nd4j.linalg.activations.Activation
import org.nd4j.linalg.learning.config.Nesterovs
import org.nd4j.linalg.lossfunctions.LossFunctions
import org.nd4j.linalg.schedule.{MapSchedule, ScheduleType}

object Model {

  import Env._
  import Params._

  val conf: MultiLayerConfiguration = new NeuralNetConfiguration.Builder()
    .seed(seed)
    .l2(l2Value)
    .weightInit(WeightInit.XAVIER)
    .optimizationAlgo(OptimizationAlgorithm.STOCHASTIC_GRADIENT_DESCENT)
    .updater(new Nesterovs(new MapSchedule(ScheduleType.ITERATION, learningRateSchedule)))
    .list()
    .layer(
    0, new ConvolutionLayer.Builder(5, 5)
      .nIn(channelsNb)
      .stride(1, 1)
      .nOut(16)
      .activation(Activation.RELU)
      .build()
  )
    .layer(
      1, new SubsamplingLayer.Builder(SubsamplingLayer.PoolingType.MAX)
        .kernelSize(2, 2)
        .stride(2, 2)
        .build()
    )
    .layer(
      2, new ConvolutionLayer.Builder(5, 5)
        .stride(1, 1)
        .nOut(64)
        .activation(Activation.RELU)
        .build()
    )
    .layer(
      3, new SubsamplingLayer.Builder(SubsamplingLayer.PoolingType.MAX)
        .kernelSize(2, 2)
        .stride(2, 2)
        .build()
    )
    .layer(
      4, new ConvolutionLayer.Builder(5, 5)
        .stride(1, 1)
        .nOut(256)
        .activation(Activation.RELU)
        .build()
    )
    .layer(
      5, new SubsamplingLayer.Builder(SubsamplingLayer.PoolingType.MAX)
        .kernelSize(2, 2)
        .stride(2, 2)
        .build()
    )
//    .layer(
//      6, new ConvolutionLayer.Builder(5, 5)
//        .stride(1, 1)
//        .nOut(270)
//        .activation(Activation.RELU)
//        .build()
//    )
//    .layer(
//      7, new SubsamplingLayer.Builder(SubsamplingLayer.PoolingType.MAX)
//        .kernelSize(2, 2)
//        .stride(2, 2)
//        .build()
//    )
    .layer(
      6, new DenseLayer.Builder()
        .activation(Activation.RELU)
        .nOut(1024)
        .build()
    )
    .layer(
      7, new OutputLayer.Builder(LossFunctions.LossFunction.NEGATIVELOGLIKELIHOOD)
        .nOut(classesNb)
        .activation(Activation.SOFTMAX)
        .build()
    )
    .setInputType(InputType.convolutionalFlat(height, width, channelsNb))
    .build()

  val model = new MultiLayerNetwork(conf)

  model.init()
}
