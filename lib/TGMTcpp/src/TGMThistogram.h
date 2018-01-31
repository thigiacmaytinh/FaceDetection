#pragma once
#include "stdafx.h"
class TGMThistogram
{
public:
	//TGMThistogram();
	//~TGMThistogram();

	enum HistogramType
	{
		
	};

	static cv::Mat DrawHistogram(cv::Mat matInput, bool drawOnInputMat = false);
	static void ShowHistogram(cv::Mat matInput);
};

