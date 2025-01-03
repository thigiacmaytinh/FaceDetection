Load Caffe framework models  {#tutorial_dnn_googlenet}
===========================

Introduction
------------

In this tutorial you will learn how to use opencv_dnn module for image classification by using
GoogLeNet trained network from [Caffe model zoo](http://caffe.berkeleyvision.org/model_zoo.html).

We will demonstrate results of this example on the following picture.
![Buran space shuttle](images/space_shuttle.jpg)

Source Code
-----------

We will be using snippets from the example application, that can be downloaded [here](https://github.com/ludv1x/opencv_contrib/blob/master/modules/dnn/samples/caffe_googlenet.cpp).

@include dnn/samples/caffe_googlenet.cpp

Explanation
-----------

-# Firstly, download GoogLeNet model files:
   [bvlc_googlenet.prototxt  ](https://raw.githubusercontent.com/ludv1x/opencv_contrib/master/modules/dnn/samples/bvlc_googlenet.prototxt) and
   [bvlc_googlenet.caffemodel](http://dl.caffe.berkeleyvision.org/bvlc_googlenet.caffemodel)

   Also you need file with names of [ILSVRC2012](http://image-net.org/challenges/LSVRC/2012/browse-synsets) classes:
   [synset_words.txt](https://raw.githubusercontent.com/ludv1x/opencv_contrib/master/modules/dnn/samples/synset_words.txt).

   Put these files into working dir of this program example.

-# Create the importer of Caffe models
   @snippet dnn/samples/caffe_googlenet.cpp Create the importer of Caffe model

-# Create the network and initialize its by using the created importer
   @snippet dnn/samples/caffe_googlenet.cpp Initialize network

-# Read input image and convert to the blob, acceptable by GoogleNet
   @snippet dnn/samples/caffe_googlenet.cpp Prepare blob
   Firstly, we resize the image and change its channel sequence order.

   Now image is actually a 3-dimensional array with 224x224x3 shape.

   Next, we convert the image to 4-dimensional blob (so-called batch) with 1x2x224x224 shape by using special @ref cv::dnn::Blob constructor.

-# Pass the blob to the network
   @snippet dnn/samples/caffe_googlenet.cpp Set input blob
   In bvlc_googlenet.prototxt the network input blob named as "data", therefore this blob labeled as ".data" in opencv_dnn API.

   Other blobs labeled as "name_of_layer.name_of_layer_output".

-# Make forward pass
   @snippet dnn/samples/caffe_googlenet.cpp Make forward pass
   During the forward pass output of each network layer is computed, but in this example we need output from "prob" layer only.

-# Determine the best class
   @snippet dnn/samples/caffe_googlenet.cpp Gather output
   We put the output of "prob" layer, which contain probabilities for each of 1000 ILSVRC2012 image classes, to the `prob` blob.
   And find the index of element with maximal value in this one. This index correspond to the class of the image.

-# Print results
   @snippet dnn/samples/caffe_googlenet.cpp Print results
   For our image we get:
> Best class: #812 'space shuttle'
>
> Probability: 99.6378%