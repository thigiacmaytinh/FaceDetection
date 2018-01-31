// This is the main DLL file.

#include "TGMTbridge.h"
#include "TGMTbridgeUtil.h"
#include <msclr/marshal.h>

using namespace TGMT;
using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Threading;
using namespace System::Runtime::InteropServices;


typedef unsigned char byte;

Bitmap^ TGMTbridge::MatToBitmap(cv::Mat img)
{
	if (img.data == nullptr)
		return nullptr;
	if (img.type() != CV_8UC3)
	{
		throw gcnew NotSupportedException("Only images of type CV_8UC3 are supported for conversion to Bitmap");
	}

	//create the bitmap and get the pointer to the data
	Bitmap ^bmpimg = gcnew Bitmap(img.cols, img.rows, PixelFormat::Format24bppRgb);

	BitmapData ^data = bmpimg->LockBits(System::Drawing::Rectangle(0, 0, img.cols, img.rows), ImageLockMode::WriteOnly, PixelFormat::Format24bppRgb);

	byte *dstData = reinterpret_cast<byte*>(data->Scan0.ToPointer());

	unsigned char *srcData = img.data;

	for (int row = 0; row < data->Height; ++row)
	{
		memcpy(reinterpret_cast<void*>(&dstData[row*data->Stride]), reinterpret_cast<void*>(&srcData[row*img.step]), img.cols*img.channels());
	}

	bmpimg->UnlockBits(data);
	delete(data);
	img.release();
	return bmpimg;	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define USE_OLD_WAY

cv::Mat TGMTbridge::BitmapToMat(Bitmap^ bitmap)
{
	
#ifdef USE_OLD_WAY
	IplImage* tmp;

	BitmapData^ bmData = bitmap->LockBits(System::Drawing::Rectangle(0, 0, bitmap->Width, bitmap->Height), ImageLockMode::ReadWrite, bitmap->PixelFormat);
	if (bitmap->PixelFormat == PixelFormat::Format8bppIndexed)
	{
		tmp = cvCreateImage(cvSize(bitmap->Width, bitmap->Height), IPL_DEPTH_8U, 1);
		tmp->imageData = (char*)bmData->Scan0.ToPointer();
	}

	else if (bitmap->PixelFormat == PixelFormat::Format24bppRgb)
	{
		tmp = cvCreateImage(cvSize(bitmap->Width, bitmap->Height), IPL_DEPTH_8U, 3);
		tmp->imageData = (char*)bmData->Scan0.ToPointer();
	}

	bitmap->UnlockBits(bmData);
	delete(bmData);
#if CV_MAJOR_VERSION == 3
	return cv::cvarrToMat(tmp);
#else
	return cv::Mat(tmp);
#endif
#else
	Drawing::Rectangle rect = Drawing::Rectangle(0, 0, bitmap->Width, bitmap->Height);

	BitmapData^ bmpData = bitmap->LockBits(rect, ImageLockMode::ReadWrite, bitmap->PixelFormat);

	// data = scan0 is a pointer to our memory block.
	IntPtr data = bmpData->Scan0;

	// step = stride = amount of bytes for a single line of the image
	size_t step = bmpData->Stride;
	
	// So you can try to get you Mat instance like this:
	cv::Mat mat; 

	if (bitmap->PixelFormat == PixelFormat::Format8bppIndexed)
	{
		mat = cv::Mat(bitmap->Height, bitmap->Width, CV_8UC1, data.ToPointer(), step);
	}
	else if (bitmap->PixelFormat == PixelFormat::Format24bppRgb)
	{
		mat = cv::Mat(bitmap->Height, bitmap->Width, CV_8UC3, data.ToPointer(), step);
	}
	else
	{
		TGMTbridgeUtil::ShowErrorBox("IPSS error", "Does not support input image because don't know format");
	}

	// Unlock the bits.
	bitmap->UnlockBits(bmpData);
	delete(bmpData);
	
	return mat;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Point2f TGMTbridge::Point2cvPoint(System::Drawing::Point^ p)
{
	return cv::Point2f(p->X, p->Y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Rect TGMTbridge::RectangleToCvRect(Drawing::Rectangle^ rect)
{
	return cv::Rect(rect->X, rect->Y, rect->Width, rect->Height);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Drawing::Rectangle TGMTbridge::CvRectToRectangle(cv::Rect rect)
{
	return Drawing::Rectangle(rect.x, rect.y, rect.width, rect.height);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTbridge::StringToCharPointer(System::String^ string, char* output, int size)
{
	pin_ptr<const __wchar_t> p = PtrToStringChars(string);
	wcstombs_s(NULL, output, size, p, size);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

System::String^ TGMTbridge::stdStrToSystemStr(std::string str)
{
	return gcnew System::String(str.c_str());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTbridge::SystemStr2stdStr(System::String^ str)
{

	const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(str)).ToPointer();
	std::string result = chars;
	Marshal::FreeHGlobal(System::IntPtr((void*)chars));
	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char* TGMTbridge::SystemStringToChar(System::String^ str)
{
	return (char*)(void*)Marshal::StringToHGlobalAnsi(str);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int TGMTbridge::SystemStringToInt(System::String^ str)
{
	return int::Parse(str);
}