#pragma once
#include "stdafx.h"
#include "TGMTshape.h"

class TGMTdraw
{
	static void PutText(cv::Mat& matInput, cv::Point p, cv::Scalar color, float fontScale, char* text);
public:
	static void DrawLine(cv::Mat& matInput,cv::Point pt1, cv::Point pt2, cv::Scalar color);
	static void DrawLine(cv::Mat& matInput,cv::Point pt1, cv::Point pt2, cv::Scalar color, int thickness);
	static void DrawLine(cv::Mat& matInput,cv::Point pt1, cv::Point pt2, cv::Scalar color, int thickness, int lineType);
	static void DrawLine(cv::Mat& matInput, cv::Point pt1, cv::Point pt2, cv::Scalar color, int thickness, int lineType, int shift);
	static void DrawLine(cv::Mat& imgInput, int rhoValue, int thetaValue);
	static void DrawLine(cv::Mat& matInput, cv::Vec2f line, cv::Scalar color = RED);
	static void DrawLine(cv::Mat& matInput, cv::Vec4i line, cv::Scalar color = RED);
	static void DrawLines(cv::Mat& matInput, std::vector<cv::Vec2f> lines, cv::Scalar color = RED);
	static void DrawLines(cv::Mat& matInput, std::vector<cv::Vec4i> lines, cv::Scalar color = RED);

	static void DrawRectangle(cv::Mat& matInput, cv::Rect rect, cv::Scalar color = RED);
	static void DrawRectangle(cv::Mat& matInput, cv::Rect rect, cv::Scalar color, int thickness);
	static void DrawRectangle(cv::Mat& matInput, cv::Rect rect, cv::Scalar color, int thickness, int lineType);
	static void DrawRectangle(cv::Mat& matInput, cv::Rect rect, cv::Scalar color, int thickness, int lineType, int shift);

	static void DrawRectangles(cv::Mat& matInput, std::vector<cv::Rect> rects, int thickness = 1, cv::Scalar color = UNDEFINED_COLOR);

	static void DrawRotatedRectangle(cv::Mat& matInput, cv::RotatedRect rect, int thickness = 1, cv::Scalar color = UNDEFINED_COLOR);
	static void DrawRotatedRectangles(cv::Mat& matInput, std::vector<cv::RotatedRect> rects, int thickness = 1, cv::Scalar color = UNDEFINED_COLOR);

	static void PutText(cv::Mat& matInput, cv::Point p, cv::Scalar color, const char* fmt, ...);
	static void PutText(cv::Mat& matInput, cv::Point p, cv::Scalar color, float fontScale, const char* fmt, ...);

	
	static void DrawRectMask(cv::Mat& matInput, cv::Rect rect, float alpha);
	
	static void DrawCircle(cv::Mat& matInput, TGMTshape::Circle circle, cv::Scalar color = UNDEFINED_COLOR, int thickness = -1);
	static void DrawCircles(cv::Mat& matInput, std::vector<TGMTshape::Circle> circles, cv::Scalar color = UNDEFINED_COLOR, int thickness = -1);

	static void DrawNoticeBox(cv::Mat& matInput, cv::Rect rect, cv::Scalar color, int thickness, const char* fmt, ...);

	static void DrawSquares(cv::Mat matInput, const std::vector<std::vector<cv::Point> > squares);
};

