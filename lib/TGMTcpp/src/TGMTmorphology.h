#pragma once
#include "stdafx.h"

class TGMTmorphology
{
public:
	//TGMTmorphology();
	//~TGMTmorphology();
	static cv::Mat Dilate(cv::Mat& mat, int type, int size);
	static cv::Mat Erode(cv::Mat& mat, int type, int size);
	static cv::Mat RemoveNoise(cv::Mat matInput, int size = 3);
	static cv::Mat FillSmallHole(cv::Mat matInput, int size = 3);
};

