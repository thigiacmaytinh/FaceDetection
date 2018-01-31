#pragma once
#include "stdafx.h"
#include "TGMTml.h"



#define GetTGMTknn TGMTknn::GetInstance

class TGMTknn : public TGMTml
{
	bool m_isTrained = false;	

#if CV_MAJOR_VERSION == 3
	cv::Ptr<KNearest> knn;
#else
	cv::KNearest    *knn;
#endif

	static TGMTknn* m_instance;
public:
	TGMTknn();
	~TGMTknn();

	static TGMTknn* GetInstance()
	{
		if (!m_instance)
			m_instance = new TGMTknn();
		return m_instance;
	}
	using TGMTml::TrainData;
	bool TrainData(cv::Mat matData, cv::Mat matLabel) override;

	float Predict(cv::Mat matInput) override;
	float Predict(std::string imgPath) override;

	
	void SaveModel(std::string fileName);
	bool LoadModel(std::string fileName);
};

