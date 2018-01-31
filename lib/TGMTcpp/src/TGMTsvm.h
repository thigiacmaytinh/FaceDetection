#pragma once
#include "stdafx.h"
#include "TGMTml.h"


#define GetTGMTsvm TGMTsvm::GetInstance

class TGMTsvm : public TGMTml
{
	static TGMTsvm* m_instance;
	

#if CV_MAJOR_VERSION == 3
	cv::Ptr<SVM> svm;
#else
	CvSVM svm;
#endif
	bool m_isTrained = false;

	
public:
	TGMTsvm();
	~TGMTsvm();
	
	static TGMTsvm* GetInstance()
	{
		if (!m_instance)
			m_instance = new TGMTsvm();
		return m_instance;
	}
	using TGMTml::TrainData;
	bool TrainData(cv::Mat matData, cv::Mat matLabel) override;

	float Predict(cv::Mat matInput) override;
	float Predict(std::string imgPath) override;

	void SaveModel(std::string filePath);
	bool LoadModel(std::string filePath);
};

