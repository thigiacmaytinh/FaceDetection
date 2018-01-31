#include "TGMThistogram.h"
#include "TGMTdebugger.h"

//TGMThistogram::TGMThistogram()
//{
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//TGMThistogram::~TGMThistogram()
//{
//}

////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat TGMThistogram::DrawHistogram(cv::Mat matInput, bool drawOnInputMat)
{
	if (!matInput.data)
	{
		PrintError("image input error");
		return cv::Mat();
	}

	std::vector<cv::Mat> bgr_planes;
	cv::split(matInput, bgr_planes);

	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true;
	bool accumulate = false;

	cv::Mat b_hist, g_hist, r_hist;

	/// Compute the histograms:
	cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	if (matInput.channels() == 3)
	{
		cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
		cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
	}


	// Draw the histograms for B, G and R
	int hist_w = matInput.cols;
	int hist_h = matInput.rows;
	int bin_w = cvRound((double)hist_w / histSize);

	cv::Mat histImage(hist_w, hist_h, CV_8UC3, cv::Scalar(0, 0, 0));
	if (drawOnInputMat)
	{
		histImage = matInput.clone();
	}

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

	/// Draw for each channel
	for (int i = 1; i < histSize; i++)
	{
		cv::line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			cv::Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			matInput.channels() == 3 ? BLUE : WHITE, 1, 8, 0);
		if (matInput.channels() == 3)
		{
			cv::line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
				cv::Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
				GREEN, 1, 8, 0);
			cv::line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
				cv::Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
				RED, 1, 8, 0);
		}
	}

	return histImage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMThistogram::ShowHistogram(cv::Mat matInput)
{
	cv::imshow("Histogram", DrawHistogram(matInput));
}