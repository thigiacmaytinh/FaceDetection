#pragma once
#include "stdafx.h"
#include <opencv2/video.hpp>


#define GetTGMTbgs TGMTbgs::GetInstance

#ifdef _MANAGED
using namespace System::Drawing;

namespace TGMT
{
	public ref class TGMTbgsBridge
	{
	public:
		void Init(int type);
		void Process(Bitmap^ bmp);
		//void Process(System::String^ videoPath);
	};
}
#endif

class TGMTbgs
{
	static TGMTbgs* instance;
	int m_blurSize = 0;
	cv::Ptr<cv::BackgroundSubtractor> m_pMOG2;
public:
	enum BgsType
	{

	};

	TGMTbgs();
	~TGMTbgs();




	static TGMTbgs* GetInstance()
	{
		if (!instance)
			instance = new TGMTbgs();
		return instance;
	}


	void Init(BgsType type);
	void Process(cv::Mat matInput);
	//void Process(std::string videoPath);
	bool SetBlurSize(int size);
};
