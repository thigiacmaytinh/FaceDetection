#pragma once
#include "stdafx.h"

class TGMTblob
{
public:
	//TGMTblob(void);
	//~TGMTblob(void);
	struct Blob
	{
		cv::Size matContainSize;
		cv::Rect boundingRect;
		std::vector<cv::Point2i> points;

		Blob(Blob* b);
		Blob() {};

		//return binary mat
		cv::Mat ToMat();
	};

	static void Demo(std::string imgPath);
	static std::vector<Blob> FindBlobs(const cv::Mat &matBinary, cv::Size minSize = cv::Size(0, 0), cv::Size maxSize = cv::Size(0, 0));
	

	static Blob FindBiggestBlob(std::vector<Blob> blobs);
	//point: move start rect a delta point
	static cv::Mat DrawBlobs(cv::Mat matInput, std::vector<Blob> blobs, cv::Point startPoint = cv::Point(0,0), cv::Scalar color = UNDEFINED_COLOR);
	static cv::Mat DrawBlob(cv::Mat matInput, Blob blob, cv::Point startPoint = cv::Point(0, 0), cv::Scalar color = UNDEFINED_COLOR);
	static cv::Mat DrawBoundingRects(cv::Mat matInput, std::vector<Blob> blobs, cv::Point startPoint = cv::Point(0, 0), cv::Scalar color = UNDEFINED_COLOR, int thickness =1);

	static cv::Mat DrawBlobAndTrim(Blob blob, cv::Scalar color = WHITE);
	static Blob Normalize(Blob &blob);
	static std::vector<cv::Point> MergeBlobs(std::vector<TGMTblob::Blob> blobs);
	static Blob GetBlobBoundary(cv::Mat matInput, TGMTblob::Blob blob);
	static cv::Point2f GetCenterPoint(Blob blob);
};

