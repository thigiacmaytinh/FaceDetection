/*****************************************************************************
*   Number Plate Recognition using SVM and Neural Networks
******************************************************************************
*   by David Millán Escrivá, 5th Dec 2012
*   http://blog.damiles.com
******************************************************************************
*   Ch5 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*   http://www.packtpub.com/cool-projects-with-opencv/book
*****************************************************************************/

#pragma once


#include "stdafx.h"
#include "TGMTml.h"

#define CHAR_SIZE	20

#define GetTGMTocr TGMTocr::GetInstance

class TGMTocr
{
	static TGMTocr* m_instance;

	enum ModelType
	{
		SVM = 0,
		KNN,
		TESS
	}m_modelType;

	

	TGMTml* m_pTGMTml;
	bool m_initSuccess;
public:

	static TGMTocr* GetInstance()
	{
		if (!m_instance)
			m_instance = new TGMTocr();
		return m_instance;
	}

	
	static const char strCharacters[];

	TGMTocr();

	
	//int ClassifyKnn(cv::Mat fmat, cv::Size charSize);
	//void TrainKnn(cv::Mat trainSamples, cv::Mat trainClasses);

	float Predict(cv::Mat matInput);
	float Predict(std::string imgPath);

	bool Training(std::string inputDir);

	bool LoadData(std::string dataFile);
	void SaveData(std::string outputFile);
private:
	bool m_trained;

	//
	//cv::Ptr<KNearest> knn;

	//convert image to squad image
	cv::Mat PreprocessChar(cv::Mat in);
	
	
};
