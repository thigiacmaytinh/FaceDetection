#pragma once
#include "stdafx.h"

using namespace System::Drawing;

#define Str2str TGMTbridge::SystemStr2stdStr
#define str2Str TGMTbridge::stdStrToSystemStr

namespace TGMT 
{
	ref class TGMTbridge
	{
	public:
		static Bitmap^ MatToBitmap(const cv::Mat img);
		static cv::Mat BitmapToMat(Bitmap^ bitmap);
		static cv::Point2f Point2cvPoint(Point^ p);

		static cv::Rect RectangleToCvRect(System::Drawing::Rectangle^ rect);
		static System::Drawing::Rectangle CvRectToRectangle(cv::Rect rect);

		static void StringToCharPointer(System::String^ string, char* output, int size);
		static System::String^ stdStrToSystemStr(std::string str);
		static std::string SystemStr2stdStr(System::String^ str);
		static char* SystemStringToChar(System::String^ str);

		static int SystemStringToInt(System::String^ str);
	};
}
