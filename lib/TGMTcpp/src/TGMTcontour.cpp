#include "TGMTcontour.h"
#include "TGMTcolor.h"
#include "TGMTimage.h"
#include "TGMTshape.h"

//TGMTcontour::TGMTcontour()
//{
//}
//
//
//TGMTcontour::~TGMTcontour()
//{
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat TGMTcontour::DrawContours(const cv::Mat matInput, std::vector<TGMTcontour::Contour> contours, cv::Scalar color, int thickness)
{
	cv::Mat matDraw = matInput.clone();	
	if (matDraw.channels() == 1)
		cv::cvtColor(matDraw, matDraw, CV_GRAY2BGR);
	for (int i = 0; i < contours.size(); ++i)
	{
		cv::drawContours(matDraw, contours, i, color == UNDEFINED_COLOR ? TGMTcolor::GetRandomColor() : color, thickness);
	}
	return matDraw;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat TGMTcontour::ContourToMat(std::vector<TGMTcontour::Contour> contours, int contourID)
{
	cv::Mat matDraw = cv::Mat(cv::boundingRect(contours[contourID]).size(), CV_8UC1);
	cv::drawContours( matDraw, contours, contourID, WHITE, CV_FILLED);
	return matDraw;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTcontour::DetectAndDrawContour(cv::Mat matInput, cv::Size minSize, cv::Size maxSize)
{
	cv::Mat mat = matInput.clone();	

	if (mat.channels() == 1)
		cv::cvtColor(mat, mat, CV_GRAY2BGR);

	std::vector<Contour> contours = FindContours(matInput, false, minSize, maxSize);
	mat = DrawContours(mat, contours, UNDEFINED_COLOR);
	cv::imshow("DetectAndDrawContour", mat);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<TGMTcontour::Contour> TGMTcontour::FindContours(cv::Mat matInput, int padding, cv::Size minSize, cv::Size maxSize)
{
	std::vector<Contour> contours, result;
	cv::Mat matGray = TGMTimage::ConvertToGray(matInput);
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(matGray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); ++i)
	{
		cv::Rect r = cv::boundingRect(contours[i]);
		if (r.width > minSize.width && r.height > minSize.height
			&& ((maxSize.width == 0 && maxSize.height ==0 )|| (r.width < maxSize.width && r.height < maxSize.height))
			&& (padding == 0 || TGMTshape::IsRectInsideMat(r, matInput, padding)))
		{			
			result.push_back(contours[i]);
		}
	}
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<TGMTshape::Circle> TGMTcontour::GetEnclosingCircle(std::vector<Contour> contours)
{
	std::vector<TGMTshape::Circle> circles;
	for (int i = 0; i < contours.size(); ++i)
	{
		cv::Point2f center;
		float radius;
		cv::minEnclosingCircle(contours[i], center, radius);
		circles.push_back(TGMTshape::Circle(center, radius));
	}
	return circles;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTshape::Circle TGMTcontour::GetEnclosingCircle(Contour contour)
{
	cv::Point2f center;
	float radius;
	cv::minEnclosingCircle(contour, center, radius);
	return TGMTshape::Circle(center, radius);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat TGMTcontour::DrawBoundingRects(const cv::Mat matInput, std::vector<TGMTcontour::Contour> contours, cv::Scalar color, int thickness)
{
	cv::Mat matDraw = matInput.clone();

	if (matDraw.channels() == 1)
		cv::cvtColor(matDraw, matDraw, CV_GRAY2BGR);
	for (int i = 0; i < contours.size(); ++i)
	{
		cv::Rect r = cv::boundingRect(contours[i]);
		cv::rectangle(matDraw, r, color == UNDEFINED_COLOR ? TGMTcolor::GetRandomColor() : color, thickness);
	}
	return matDraw;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat TGMTcontour::DrawBoundingRect(const cv::Mat matInput, TGMTcontour::Contour contour, cv::Scalar color, int thickness)
{
	cv::Mat matDraw = matInput.clone();
	cv::Rect r = cv::boundingRect(contour);
	cv::rectangle(matDraw, r, color == UNDEFINED_COLOR ? TGMTcolor::GetRandomColor() : color, thickness);
	return matDraw;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTcontour::Contour TGMTcontour::GetBiggestContour(std::vector<Contour> contours)
{
	int largest_area = 0;
	int idx = 0;
	for (size_t i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i], false);  //  Find the area of contour
		if (area>largest_area)
		{
			largest_area = area;
			idx = i;
		}
	}
	return contours[idx];
}