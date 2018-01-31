#pragma once
#include "stdafx.h"

class TGMTBean
{
public:
	TGMTBean();
	~TGMTBean();

	static int CountBean(std::string);
	static int CountBean(cv::Mat);
	static std::vector<std::vector<cv::Point>> GetValidContour(std::vector<std::vector<cv::Point>> contours, cv::Size min, cv::Size max);
};

