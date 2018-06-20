#pragma once
#include <string>
#include <vector>
#include "stdafx.h"

#define GetFaceDetection FaceDetection::GetInstance

class FaceDetection
{
	static FaceDetection* m_instance;
	int m_confident;
public:
	bool m_enableDebug = false;
	
	FaceDetection();
	~FaceDetection();

	static FaceDetection* GetInstance()
	{
		if (!m_instance)
			m_instance = new FaceDetection();
		return m_instance;
	}

	//return peoples in image
	std::vector<std::string> RecogImage(cv::Mat matInput, bool cropped = false);

	// return image path and people in each image
	std::string RecogImages(std::string inputPath, std::string &errMsg, bool cropped = false);
};

