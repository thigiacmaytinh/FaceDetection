#include "TGMTann.h"
#include "TGMTdebugger.h"

const int TGMTann::numCharacters = 30;

////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTann::TGMTann()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTann::~TGMTann()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTann::TrainAnn(cv::Mat TrainData, cv::Mat classes, int nlayers)
{
	DEBUG_IMAGE(TrainData, "TrainData");

	cv::Mat_<int> layers(3, 1);
	layers(0) = TrainData.cols;     // input
	layers(1) = numCharacters;  // hidden
	layers(2) = nlayers;  // hidden


	ann = ANN_MLP::create();
	ann->setLayerSizes(layers);
	ann->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM);

	//Prepare trainClases
	//Create a mat with n trained data by m classes
	cv::Mat trainClasses;
	trainClasses.create(TrainData.rows, nlayers, CV_32FC1);
	for (int i = 0; i < trainClasses.rows; i++)
	{
		for (int k = 0; k < trainClasses.cols; k++)
		{
			//If class of data i is same than a k class
			if (k == classes.at<int>(i))
				trainClasses.at<float>(i, k) = 1;
			else
				trainClasses.at<float>(i, k) = 0;
		}
	}


	//Learn classifier
	m_isTrained = ann->train(TrainData, cv::ml::ROW_SAMPLE, trainClasses);

	ASSERT(m_isTrained, "Training data TGMTocr failed");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int TGMTann::ClassifyAnn(cv::Mat matInput)
{
	cv::Mat mat = PrepareMatData(matInput);

	int result = -1;
	cv::Mat output(1, numCharacters, CV_32FC1);

	ann->predict(mat, output);

	cv::Point maxLoc;
	double maxVal;
	cv::minMaxLoc(output, 0, &maxVal, 0, &maxLoc);
	//We need know where in output is the max val, the x (cols) is the class.

	return maxLoc.x;
}
