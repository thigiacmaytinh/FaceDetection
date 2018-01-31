#include "stdafx.h"
#include "TGMTblur.h"

#define DELAY_BLUR 100
#define MAX_KERNEL_LENGTH 31


//TGMTblur::TGMTblur()
//{
//}
//
//
//TGMTblur::~TGMTblur()
//{
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int DisplayImage(char* windowName, cv::Mat dst)
{
	cv::imshow(windowName, dst);
	int c = cv::waitKey(DELAY_BLUR);
	if (c >= 0) { return -1; }
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTblur::Demo(std::string imgPath)
{
	cv::namedWindow("Filter Demo", CV_WINDOW_AUTOSIZE);

	/// Load the source image
	cv::Mat src = cv::imread("a.jpg", 1);

	cv::Mat dst = src.clone();
	if (DisplayImage("Original Image", dst) != 0) { return; }

	/// Applying Homogeneous blur
	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		blur(src, dst, cv::Size(i, i), cv::Point(-1, -1));
		if (DisplayImage("Homogeneous Blur", dst) != 0) { return; }
	}

	/// Applying Gaussian blur
	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		GaussianBlur(src, dst, cv::Size(i, i), 0, 0);
		if (DisplayImage("Gaussian Blur", dst) != 0) { return; }
	}

	/// Applying Median blur
	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		medianBlur(src, dst, i);
		if (DisplayImage("Median Blur", dst) != 0) { return; }
	}

	/// Applying Bilateral Filter
	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		bilateralFilter(src, dst, i, i * 2, i / 2);
		if (DisplayImage("Bilateral Blur", dst) != 0) { return; }
	}
}