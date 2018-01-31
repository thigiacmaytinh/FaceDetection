#include "TGMTBean.h"
#include "TGMTfile.h"
#include "TGMTimage.h"
#include "TGMTcontour.h"

TGMTBean::TGMTBean()
{
}


TGMTBean::~TGMTBean()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int TGMTBean::CountBean(std::string filePath)
{
	if (!TGMTfile::FileExist(filePath))
		return 0;
	return CountBean(cv::imread(filePath, CV_LOAD_IMAGE_GRAYSCALE));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int TGMTBean::CountBean(cv::Mat img)
{
	if (!img.data) return 0;

	cv::Mat opening, sure_bg, sure_fg, dist_transform, unknown;
	cv::Mat gray, binary;

	if (img.channels() > 1)
		cv::cvtColor(img, gray, CV_BGR2GRAY);
	else
		gray = img;
	imshow("gray", gray);

	cv::GaussianBlur(gray, gray, cv::Size(9, 9), 0, 0);
	imshow("blur", gray);

	cv::adaptiveThreshold(gray, binary, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 5, 0);

	imshow("binary", binary);


	//noise removal
	cv::Mat kernel = cv::Mat::ones(cv::Size(3, 3), CV_8U);
	morphologyEx(binary, opening, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), 2);
	imshow("opening", opening);


	////Finding sure foreground area
	//cv::distanceTransform(opening, dist_transform, CV_DIST_L2, 5);
	//imshow("dist_transform", dist_transform);

	//cv::Mat matNorm;
	//cv::normalize(dist_transform, matNorm, 0, 1., cv::NORM_MINMAX);
	//imshow("matNorm", matNorm);

	//double min, max;
	//cv::minMaxLoc(dist_transform, &min, &max);
	//cv::threshold(dist_transform, sure_fg, 0.4*max, 255, 0);
	//imshow("sure_fg", sure_fg);
	//

	//cv::Mat newMat;
	//sure_fg.convertTo(newMat, CV_8U);


	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(opening, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	contours = GetValidContour(contours, cv::Size(5, 5), cv::Size(20, 20));

	cv::Mat matDraw = TGMTcontour::DrawContours(img, contours);
	imshow("matDraw", matDraw);


	return contours.size();
}

std::vector<std::vector<cv::Point>> TGMTBean::GetValidContour(std::vector<std::vector<cv::Point>> contours, cv::Size min, cv::Size max)
{
	std::vector<std::vector<cv::Point>> result;
	for (size_t i = 0; i < contours.size(); ++i)
	{
		cv::Rect r = cv::boundingRect(contours[i]);
		if (min.width <= r.width && min.height <= r.height && r.width <= max.width && r.height <= max.height)
		{
			result.push_back(contours[i]);
		}
	}
	return result;
}