#include "TGMTdraw.h"
#if defined(WIN32) || defined(WIN64)
#include <Windows.h>
#endif
#include "TGMTcolor.h"

//TGMTdraw::TGMTdraw()
//{
//}
//
//
//TGMTdraw::~TGMTdraw()
//{
//}



void TGMTdraw::DrawLine(cv::Mat& matInput, cv::Point pt1, cv::Point pt2, cv::Scalar color)
{
	DrawLine(matInput, pt1, pt2, color, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawLine(cv::Mat& matInput, cv::Point pt1, cv::Point pt2, cv::Scalar color, int thickness)
{
	DrawLine(matInput,pt1, pt2, color, thickness, 8);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawLine(cv::Mat& matInput, cv::Point pt1, cv::Point pt2, cv::Scalar color, int thickness, int lineType)
{
	DrawLine(matInput,pt1, pt2, color, thickness, lineType, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawLine(cv::Mat& matInput, cv::Point pt1, cv::Point pt2, cv::Scalar color, int thickness, int lineType, int shift)
{
	cv::line(matInput, pt1, pt2, color, thickness, lineType, shift);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawLine(cv::Mat& matInput, cv::Vec2f line, cv::Scalar color)
{
	if (!matInput.data)
		return;


	if (line[1] != 0)
	{
		float m = -1 / tan(line[1]);
		float c = line[0] / sin(line[1]);
		cv::line(matInput, cv::Point(0, c), cv::Point(matInput.cols, m*matInput.cols + c), color);
	}
	else
	{
		cv::line(matInput, cv::Point(line[0], 0), cv::Point(line[0], matInput.rows), color);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawLine(cv::Mat& matInput, cv::Vec4i line, cv::Scalar color)
{
	if (!matInput.data)
		return;

	cv::line(matInput, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), color, 1, CV_AA);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawLines(cv::Mat& matInput, std::vector<cv::Vec2f> lines, cv::Scalar color)
{
	if (!matInput.data)
		return;

	for (int i = 0; i < lines.size(); ++i)
	{
		cv::Vec2f line = lines[i];
		if (line[1] != 0)
		{
			float m = -1 / tan(line[1]);
			float c = line[0] / sin(line[1]);
			cv::line(matInput, cv::Point(0, c), cv::Point(matInput.cols, m*matInput.cols + c), color);
		}
		else
		{
			cv::line(matInput, cv::Point(line[0], 0), cv::Point(line[0], matInput.rows), color);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawLines(cv::Mat& img, std::vector<cv::Vec4i> lines, cv::Scalar color)
{
	if (!img.data)
		return;

	for (int i = 0; i < lines.size(); i++)
	{
		DrawLine(img, lines[i], color);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawLine(cv::Mat& imgInput, int rhoValue, int thetaValue)
{
	cv::Vec2f line = cv::Vec2f(rhoValue, thetaValue);
	DrawLine(imgInput, line);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawRectangle(cv::Mat& matInput, cv::Rect rect, cv::Scalar color)
{
	return DrawRectangle(matInput, rect, color, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawRectangle(cv::Mat& matInput, cv::Rect rect, cv::Scalar color, int thickness)
{
	return DrawRectangle(matInput, rect, color, thickness, 8, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawRectangle(cv::Mat& matInput, cv::Rect rect, cv::Scalar color, int thickness, int lineType)
{
	return DrawRectangle(matInput, rect, color, thickness, lineType, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawRectangle(cv::Mat& matInput, cv::Rect rect, cv::Scalar color, int thickness, int lineType, int shift)
{
	cv::rectangle(matInput, rect.tl(), rect.br(), color, thickness, lineType, shift);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawRectangles(cv::Mat& matInput, std::vector<cv::Rect> rects, int thickness, cv::Scalar color)
{
	for (int i = 0; i < rects.size(); i++)
	{
		if (color == UNDEFINED_COLOR)
			DrawRectangle(matInput, rects[i],  TGMTcolor::GetRandomColor(), thickness);
		else
			DrawRectangle(matInput, rects[i], color, thickness);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::PutText(cv::Mat& img, cv::Point p, cv::Scalar color, const char* fmt, ...)
{
	char str[DEBUG_OUT_BUFFER_SIZE];
	va_list arg_list;
#ifndef _ARM_
	va_start(arg_list, fmt);
#else
	__crt_va_start_a(arg_list, fmt);
#endif
	
	vsnprintf(str, DEBUG_OUT_BUFFER_SIZE - 1, fmt, arg_list);


	PutText(img, p, color, 1.f, str);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::PutText(cv::Mat& img, cv::Point p, cv::Scalar color, float fontScale, const char* fmt, ...)
{
	char str[DEBUG_OUT_BUFFER_SIZE];
	va_list arg_list;
#ifndef _ARM_
	va_start(arg_list, fmt);
#else
	__crt_va_start_a(arg_list, fmt);
#endif
	vsnprintf(str, DEBUG_OUT_BUFFER_SIZE - 1, fmt, arg_list);

	PutText(img, p, color, fontScale, str);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::PutText(cv::Mat& matInput, cv::Point p, cv::Scalar color, float fontScale, char* text)
{
	if (text == nullptr || text == "")
		return;

	cv::putText(matInput, text, p, cv::FONT_ITALIC, fontScale, color, 2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawRotatedRectangle(cv::Mat& matInput, cv::RotatedRect rect, int thickness, cv::Scalar color)
{
	if (matInput.channels() == 1)
		cv::cvtColor(matInput, matInput, CV_GRAY2BGR);
	cv::Point2f rectPoints[4]; 
	rect.points(rectPoints);

	cv::Scalar clr;
	if (color == UNDEFINED_COLOR)
		clr = TGMTcolor::GetRandomColor();
	for (int j = 0; j < 4; j++)
	{
		line(matInput, rectPoints[j], rectPoints[(j + 1) % 4], clr, thickness, 8);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawRotatedRectangles(cv::Mat& matInput, std::vector<cv::RotatedRect> rects, int thickness, cv::Scalar color)
{
	if (matInput.channels() == 1)
		cv::cvtColor(matInput, matInput, CV_GRAY2BGR);

	for (int i = 0; i < rects.size(); i++)
	{
		cv::Point2f rectPoints[4];
		rects[i].points(rectPoints);

		cv::Scalar clr;
		if(color == UNDEFINED_COLOR)
			clr = TGMTcolor::GetRandomColor();
		for (int j = 0; j < 4; j++)
		{
			cv::line(matInput, rectPoints[j], rectPoints[(j + 1) % 4], clr, thickness, 8);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawRectMask(cv::Mat& matInput, cv::Rect rect, float alpha)
{
	cv::Mat mask(matInput.size(), CV_8UC3, BLACK);
	cv::rectangle(mask, rect, WHITE, -1);

	cv::Mat matRoi;
	cv::bitwise_and(matInput, mask, matRoi);


	cv::Mat matBlur(matInput.size(), CV_8UC3);
	cv::addWeighted(matInput, 1 - alpha, cv::Scalar(50, 50, 50), alpha, 0.0, matBlur);
	cv::addWeighted(matRoi, 1, matBlur, 1, 0.0, matRoi);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawCircle(cv::Mat& matInput, TGMTshape::Circle circle, cv::Scalar color, int thickness)
{
	cv::circle(matInput, circle.center, circle.radius,color, thickness, 8, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawCircles(cv::Mat& matInput, std::vector<TGMTshape::Circle> circles, cv::Scalar color, int thickness)
{
	for (size_t i = 0; i < circles.size(); i++)
	{
		cv::circle(matInput, circles[i].center, circles[i].radius, color == UNDEFINED_COLOR ? TGMTcolor::GetRandomColor() : color, thickness, 8, 0);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawNoticeBox(cv::Mat& matInput, cv::Rect rect, cv::Scalar color, const char* fmt, ...)
{
	char str[DEBUG_OUT_BUFFER_SIZE];
	va_list arg_list;
#ifndef _ARM_
	va_start(arg_list, fmt);
#else
	__crt_va_start_a(arg_list, fmt);
#endif
	vsnprintf(str, DEBUG_OUT_BUFFER_SIZE - 1, fmt, arg_list);


	cv::rectangle(matInput, rect, color);

	cv::Point tl = rect.tl();
	if (rect.y < 20)
	{
		tl.y += 20;
	}
	cv::putText(matInput, str, tl , cv::FONT_ITALIC, 1, color);
	

}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTdraw::DrawSquares(cv::Mat matInput, const std::vector<std::vector<cv::Point> > squares)
{
	for (size_t i = 0; i < squares.size(); i++)
	{
		const cv::Point* p = &squares[i][0];
		int n = (int)squares[i].size();
		cv::polylines(matInput, &p, &n, 1, true, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
	}
}
