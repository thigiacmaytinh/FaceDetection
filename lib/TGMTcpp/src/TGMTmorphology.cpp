#include "TGMTmorphology.h"
#include "TGMTdebugger.h"

//TGMTmorphology::TGMTmorphology()
//{
//}
//
//
//TGMTmorphology::~TGMTmorphology()
//{
//}

cv::Mat TGMTmorphology::Dilate(cv::Mat& mat, int type, int size)
{
	ASSERT(mat.channels() == 1, "Input image to dilate must be 1 channel");

	ASSERT(type >= 0 && type <= 2, "Dilate type must be one of: cv::MORPH_RECT, cv::MORPH_CROSS, cv::MORPH_ELLIPSE");

	cv::Mat matResult;
	cv::Mat element = cv::getStructuringElement(type,
		cv::Size(size * 2 + 1, size * 2 + 1),
		cv::Point(size, size));
	cv::dilate(mat, matResult, element);
	return matResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat TGMTmorphology::Erode(cv::Mat& mat, int type, int size)
{
	ASSERT(mat.channels() == 1, "Input image to dilate must be 1 channel");

	ASSERT(type >= 0 && type <= 2, "Dilate type must be one of: cv::MORPH_RECT, cv::MORPH_CROSS, cv::MORPH_ELLIPSE");

	cv::Mat matResult;
	cv::Mat element = cv::getStructuringElement(type,cv::Size(size, size));
	cv::erode(mat, matResult, element);
	return matResult;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat TGMTmorphology::RemoveNoise(cv::Mat matInput, int size)
{
	cv::Mat kernel = cv::Mat::ones(cv::Size(size, size), CV_8U);
	cv::Mat matOutput;
	cv::morphologyEx(matInput, matOutput, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), 2);
	return matOutput;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat TGMTmorphology::FillSmallHole(cv::Mat matInput, int size)
{	
	ASSERT(size % 2 == 1, "Size must be odd");
	cv::Mat matResult;
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size));
	cv::morphologyEx(matInput, matResult, cv::MORPH_CLOSE, kernel);
	return matResult;
}