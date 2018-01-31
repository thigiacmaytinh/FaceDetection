#pragma once
#include "stdafx.h"
#include "TGMTshape.h"

class TGMTcontour
{
public:
	//TGMTcontour();
	//~TGMTcontour();
	typedef std::vector<cv::Point> Contour;

	static std::vector<Contour>  FindContours(cv::Mat matInput, int padding = 0, cv::Size minSize = cv::Size(0, 0), cv::Size maxSize = cv::Size(0, 0));
	static cv::Mat DrawContours(cv::Mat matInput, std::vector<Contour> contours, cv::Scalar color = UNDEFINED_COLOR, int thickness = 1);
	static cv::Mat ContourToMat(std::vector<Contour> contours, int contourID);
	static void DetectAndDrawContour(cv::Mat matInput, cv::Size minSize = cv::Size(0,0), cv::Size maxSize = cv::Size(0, 0));
	static std::vector<TGMTshape::Circle> GetEnclosingCircle(std::vector<Contour> contours);
	static TGMTshape::Circle GetEnclosingCircle(Contour contour);
	static cv::Mat DrawBoundingRects(const cv::Mat matInput, std::vector<TGMTcontour::Contour> contours, cv::Scalar color = UNDEFINED_COLOR, int thickness = 1);
	static cv::Mat DrawBoundingRect(const cv::Mat matInput, TGMTcontour::Contour contour, cv::Scalar color = UNDEFINED_COLOR, int thickness = 1);
	static Contour GetBiggestContour(std::vector<Contour> contours);
};

