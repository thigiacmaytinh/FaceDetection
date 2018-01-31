#include "TGMTobjDetect.h"
#include "TGMTfile.h"
#include "TGMTdebugger.h"
#include "TGMTimage.h"


TGMTobjDetect* TGMTobjDetect::m_instance = nullptr;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTobjDetect::TGMTobjDetect()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTobjDetect::~TGMTobjDetect()
{
	m_cascade.~CascadeClassifier();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string Mat2BinaryString(cv::Mat frame)
{
	cv::Mat frameGray, frameBW;
	//convert to gray
	cvtColor(frame, frameGray, CV_BGR2GRAY);
	adaptiveThreshold(frameGray, frameBW, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 3, 0);
	std::string result;
	for (int row = 0; row < frameBW.rows; ++row)
	{
		for (int col = 0; col < frameBW.cols; ++col)
		{
			result += frameBW.at<char>(row, col) + 1; //+1 because values in [-1;0]
		}
	}
	return result;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<cv::Rect> TGMTobjDetect::Detect(cv::Mat matInput, float scaleFactor, int minNeighbors, cv::Size minSize, cv::Size maxSize)
{
	
	ASSERT(matInput.data, "Load image failed");

	std::vector<cv::Rect> result;
	

	cv::Mat matGray = TGMTimage::ConvertToGray(matInput);

	// Enhance / Normalise the image contrast (optional)
	//cv::equalizeHist(detectImg, detectImg);

	StartCountTime("Detect");
	
	m_cascade.detectMultiScale(matGray, result, scaleFactor, minNeighbors, CV_HAAR_SCALE_IMAGE);
	StopCountTime("Detect");



	return 	result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<cv::Rect> TGMTobjDetect::Detect(std::string filePath, float scaleFactor, int minNeighbors, cv::Size minSize, cv::Size maxSize)
{
	return Detect(cv::imread(filePath), scaleFactor, minNeighbors);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTobjDetect::Init(std::string cascadeFile)
{
	m_cascade = cv::CascadeClassifier(cascadeFile);
	ASSERT(!m_cascade.empty(), "Cascade file is empty");

	return !m_cascade.empty();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MANAGED
#include "TGMTbridge.h"
using namespace System;
using namespace System::Drawing;
using namespace TGMT;

std::vector<cv::Rect> TGMTobjDetect::Detect(System::String^ filePath, float scaleFactor, int minNeighbors, cv::Size minSize, cv::Size maxSize)
{
	std::string sFilePath = TGMT::TGMTbridge::SystemStr2stdStr(filePath);
	std::vector<cv::Rect> rects = Detect(sFilePath, scaleFactor, minNeighbors, minSize, maxSize);
	return rects;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTobjDetect::Init(System::String^ cascadeFilePath)
{
	return Init(TGMT::TGMTbridge::SystemStr2stdStr(cascadeFilePath));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTobjDetect::Inited()
{
	return !m_cascade.empty();
}
#endif