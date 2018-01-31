#pragma once
#include "stdafx.h"
#include "opencv2/objdetect.hpp"

#define GetTGMTobjDetect TGMTobjDetect::GetInstance

class TGMTobjDetect
{
private:
	static TGMTobjDetect* m_instance;
	cv::CascadeClassifier m_cascade;

public:
	TGMTobjDetect();
	~TGMTobjDetect();
	bool Init(std::string cascadeFile);

	static TGMTobjDetect* GetInstance()
	{
		if (!m_instance)
			m_instance = new TGMTobjDetect();
		return m_instance;
	}
	
	std::vector<cv::Rect> Detect(std::string filePath, float scaleFactor = 1.1, int minNeighbors = 3, cv::Size minSize = cv::Size(0, 0), cv::Size maxSize = cv::Size(0, 0));
	std::vector<cv::Rect> Detect(cv::Mat matInput, float scaleFactor = 1.1, int minNeighbors = 3, cv::Size minSize = cv::Size(0, 0), cv::Size maxSize = cv::Size(0, 0));

#ifdef _MANAGED
	bool Init(System::String^ cascadeFilePath);
	bool Inited();
	std::vector<cv::Rect> Detect(System::String^ filePath, float scaleFactor = 1.1, int minNeighbors = 3, cv::Size minSize = cv::Size(0,0), cv::Size maxSize = cv::Size(0, 0));
#endif
};