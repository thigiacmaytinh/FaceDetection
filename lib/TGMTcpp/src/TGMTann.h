#pragma once
#include "stdafx.h"
#include "TGMTml.h"

class TGMTann : public TGMTml
{
	static const int numCharacters;
	cv::Ptr<ANN_MLP> ann;
	bool m_isTrained;
public:
	TGMTann();
	~TGMTann();

	int ClassifyAnn(cv::Mat mat);
	void TrainAnn(cv::Mat trainData, cv::Mat trainClasses, int nlayers);
};

