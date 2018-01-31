#include "TGMTocr.h"
#include "TGMTimage.h"
#include "TGMTdebugger.h"
#include "TGMTfile.h"
#include "TGMTConfig.h"
#include "TGMTann.h"
#include "TGMTknn.h"
#include "TGMTsvm.h"

TGMTocr* TGMTocr::m_instance = nullptr;
const char TGMTocr::strCharacters[] = { '0','1','2','3','4','5','6','7','8','9','B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'R', 'S', 'T', 'V', 'W', 'X', 'Y', 'Z' };



////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTocr::TGMTocr()
{
	int type = GetTGMTConfig()->ReadValueInt("TGMTocr", "type");
	if (type >= 0 && type < sizeof(ModelType))
	{
		m_modelType = (ModelType)type;
	}
	else
	{
		PrintError("OCR type is invalid");
	}


	switch (m_modelType)
	{
	case ModelType::SVM:
		m_pTGMTml = GetTGMTsvm();
		break;
	case ModelType::KNN:
		m_pTGMTml = GetTGMTknn();
		break;
	case ModelType::TESS:

		break;
	};

	m_initSuccess = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//void TGMTocr::TrainKnn(cv::Mat trainSamples, cv::Mat trainClasses)
//{
//	// learn classifier
//	knn = KNearest::create();
//	knn->setIsClassifier(true);
//	knn->setAlgorithmType(KNearest::Types::BRUTE_FORCE);
//	knn->train(trainSamples, ROW_SAMPLE, trainClasses);
//}

////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat TGMTocr::PreprocessChar(cv::Mat in)
{
	//Remap image
	int h = in.rows;
	int w = in.cols;
	cv::Mat transformMat = cv::Mat::eye(2, 3, CV_32F);
	int m = cv::max(w, h);
	transformMat.at<float>(0, 2) = m / 2 - w / 2;
	transformMat.at<float>(1, 2) = m / 2 - h / 2;

	cv::Mat warpImage(m, m, in.type());
	cv::warpAffine(in, warpImage, transformMat, warpImage.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0));

	cv::Mat out;
	cv::resize(warpImage, out, cv::Size(CHAR_SIZE, CHAR_SIZE));

	return out;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTocr::Training(std::string inputDir)
{
	switch (m_modelType)
	{
	case TGMTocr::SVM:
		return GetTGMTsvm()->TrainData(inputDir);
	case TGMTocr::KNN:
		return GetTGMTknn()->TrainData(inputDir);
	case TGMTocr::TESS:
		break;
	default:
		break;
	}
	

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTocr::LoadData(std::string dataFile)
{	
	return m_pTGMTml->LoadData(dataFile);	 
}

////////////////////////////////////////////////////////////////////////////////////////////////////

float TGMTocr::Predict(cv::Mat matInput)
{
	return m_pTGMTml->Predict(matInput);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

float TGMTocr::Predict(std::string imgPath)
{
	return m_pTGMTml->Predict(imgPath);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTocr::SaveData(std::string outputFile)
{
	m_pTGMTml->SaveData(outputFile);
}