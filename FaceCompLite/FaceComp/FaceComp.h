#pragma once
#include <string>
#include <vector>
#include "stdafx.h"

#define GetFaceComp FaceComp::GetInstance

class FaceComp
{
	static FaceComp* m_instance;
	std::string PrintOutputJson(std::vector<std::pair< std::string, std::string>> result, std::string errorMessage);
	int m_confident;
public:
	bool m_enableDebug = false;
	
	FaceComp();
	~FaceComp();

	static FaceComp* GetInstance()
	{
		if (!m_instance)
			m_instance = new FaceComp();
		return m_instance;
	}

	//return peoples in image
	std::vector<std::string> RecogImage(cv::Mat matInput, bool cropped = false);

	// return image path and people in each image
	std::string RecogImages(std::string inputPath, std::string &errMsg, bool cropped = false);

	//return json
	std::string RecogSingleImage(cv::Mat matInput, bool cropped = false);
	std::string RecogSingleImage(std::string imgPath, bool cropped = false);
};

