#include "TGMTimage.h"
#include "TGMTdebugger.h"

//TGMTimage::TGMTimage(void)
//{
//
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//TGMTimage::~TGMTimage(void)
//{
//
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTimage::GetImageType(cv::Mat img)
{
	std::string r = "";

	uchar depth = img.type() & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (img.type() >> CV_CN_SHIFT);

	switch (depth)
	{
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat TGMTimage::ConvertToGray(cv::Mat matInput)
{
	ASSERT(matInput.data, "Mat input is null");

	cv::Mat matGray = matInput.clone();
	if (matInput.channels() > 1)
		cv::cvtColor(matInput, matGray, CV_BGR2GRAY);
	return matGray;
}

