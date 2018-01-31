#include "TGMTeffect.h"
#include "TGMTdebugger.h"


//TGMTeffect::TGMTeffect()
//{
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//TGMTeffect::~TGMTeffect()
//{
//}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


cv::Mat TGMTeffect::ColoredDot(cv::Mat matInput, int dotSize)
{
	ASSERT(matInput.data, "Mat input is null");


	cv::Mat dstImg = cv::Mat::zeros(matInput.size(), CV_8UC3);
	cv::Mat cir = cv::Mat::zeros(matInput.size(), CV_8UC1);


	for (int i = 0; i < matInput.rows; i += dotSize)
	{
		for (int j = 0; j < matInput.cols; j += dotSize)
		{
			cv::Rect rect = cv::Rect(j, i, dotSize, dotSize) & cv::Rect(0, 0, matInput.cols, matInput.rows);

			cv::Mat sub_dst(dstImg, rect);
			sub_dst.setTo(cv::mean(matInput(rect)));

			circle(
				cir,
				cv::Point(j + dotSize / 2, i + dotSize / 2),
				dotSize / 2 - 1,
				CV_RGB(255, 255, 255), -1, CV_AA
			);
		}
	}

	cv::Mat cir_32f;
	cir.convertTo(cir_32f, CV_32F);
	normalize(cir_32f, cir_32f, 0, 1, cv::NORM_MINMAX);

	cv::Mat dst_32f;
	dstImg.convertTo(dst_32f, CV_32F);

	std::vector<cv::Mat> channels;
	split(dst_32f, channels);
	for (int i = 0; i < channels.size(); ++i)
		channels[i] = channels[i].mul(cir_32f);

	merge(channels, dst_32f);
	dst_32f.convertTo(dstImg, CV_8U);
	return dstImg;
}