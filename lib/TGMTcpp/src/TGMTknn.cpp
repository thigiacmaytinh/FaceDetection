#include "TGMTknn.h"
#include "TGMTdebugger.h"
#include "TGMTfile.h"

TGMTknn* TGMTknn::m_instance = nullptr;

////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTknn::TGMTknn()
{
#if CV_MAJOR_VERSION == 3
	knn = KNearest::create();
	knn->setIsClassifier(true);
	knn->setAlgorithmType(KNearest::Types::BRUTE_FORCE);
#else
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTknn::~TGMTknn()
{
	knn.release();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTknn::TrainData(cv::Mat matData, cv::Mat matLabel)
{
	ASSERT(matData.data, "Mat data to train is empty");
	ASSERT(matLabel.data, "Mat label to train is empty");

	SET_CONSOLE_TITLE("Traning data knn...");
	m_isTrained = knn->train(matData, ROW_SAMPLE, matLabel);
	return m_isTrained;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

float TGMTknn::Predict(std::string filePath)
{
	cv::Mat mat = cv::imread(filePath, CV_LOAD_IMAGE_GRAYSCALE);
	return Predict(mat);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

float TGMTknn::Predict(cv::Mat matInput)
{
	ASSERT(m_isTrained, "You must train KNN before use");
	cv::Mat matResults(0, 0, CV_32FC1);

	cv::Mat matData = PrepareMatData(matInput);
	return knn->findNearest(matData, knn->getDefaultK() , matResults);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTknn::SaveModel(std::string fileName)
{
	knn->save(fileName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTknn::LoadModel(std::string fileName)
{
	knn = cv::Algorithm::load<KNearest>(fileName);
	ASSERT(knn != nullptr, "Can not load file: %s", fileName.c_str());
	m_isTrained = true;
	return m_isTrained;
}