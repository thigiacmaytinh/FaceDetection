#pragma once
#include "stdafx.h"
#include <functional>

#define GetTGMTvideo TGMTvideo::GetInstance

class TGMTvideo
{
	bool m_stopVideo = false;
	cv::VideoCapture mCap;
	static TGMTvideo* instance;
	bool LoadVideoFile(std::string videoFilePath);
	
#ifdef _MANAGED
	typedef void(*OnNewFrameManaged)(cv::Mat);
#endif
public:
	int m_frameCount = 0;
	static TGMTvideo* GetInstance()
	{
		if (!instance)
			instance = new TGMTvideo();
		return instance;
	}
	TGMTvideo();
	~TGMTvideo();

#ifdef _MANAGED
	OnNewFrameManaged OnNewFrame;
#else
	std::function<void(cv::Mat)> OnNewFrame;
#endif

	

	//play video from file
	void PlayVideo(std::string videoFilePath, cv::Size maxSize = cv::Size(0, 0), int fps = 0, int startIndex = 0, int total=0);
	void StopVideo() {	m_stopVideo = true;	};

	void ExtractFrame(std::string videoFilePath, std::string outputDir, cv::Size outputSize = cv::Size(0, 0), int startIndex =0, int total = 0);
	cv::Mat GetFrameAt(std::string videoFilePath, int frameIndex);
	int GetAmountFrame(std::string videoFilePath);
};

