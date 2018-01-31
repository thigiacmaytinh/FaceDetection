#pragma once
#include "stdafx.h"

#include <opencv2/line_descriptor.hpp>

using namespace cv::line_descriptor;

#define GetTGMTline TGMTline::GetInstance

class TGMTline
{
	static TGMTline* instance;
	cv::Ptr<BinaryDescriptor> m_binaryDescriptor = BinaryDescriptor::createBinaryDescriptor();

public:
	TGMTline();
	~TGMTline();

	static TGMTline* GetInstance()
	{
		if (!instance)
			instance = new TGMTline();
		return instance;
	}

	std::vector<KeyLine> DetectLine(cv::Mat matInput);

	cv::Mat DrawLine(cv::Mat& matInput, KeyLine line, cv::Scalar color = UNDEFINED_COLOR);
	cv::Mat DrawLines(cv::Mat& matInput, std::vector<KeyLine> lines, cv::Scalar color = UNDEFINED_COLOR);

	void DetectAndDrawLine(cv::Mat& matInput);
	static cv::Point GetIntersectPoint(KeyLine line1, KeyLine line2);

};

