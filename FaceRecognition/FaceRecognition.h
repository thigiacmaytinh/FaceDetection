#pragma once
#include <string>
#include <vector>
#include "stdafx.h"

#define GetFaceComp FaceRecognition::GetInstance

class FaceRecognition
{
	static FaceRecognition* m_instance;
	int m_confident;
public:
	bool m_enableDebug = false;
	
	FaceRecognition();
	~FaceRecognition();

	static FaceRecognition* GetInstance()
	{
		if (!m_instance)
			m_instance = new FaceRecognition();
		return m_instance;
	}
	// return image path and people in each image
	std::string RecogImages(std::string inputPath, std::string &errMsg, bool cropped = false);
};

