
#include "TGMTbgs.h"
#include "stdafx.h"

#include "TGMTutil.h"
//#include "TGMTdebugger.h"
#include "TGMTfile.h"
#include "TGMTutil.h"
#include "TGMTvideo.h"
#include "TGMTConfig.h"
#include "TGMTcamera.h"
#include "TGMTbrightness.h"
#include "TGMTmorphology.h"

#ifdef _MANAGED
#include "TGMTbridge.h"

using namespace TGMT;

#endif

TGMTbgs* TGMTbgs::instance = NULL;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTbgs::TGMTbgs()
{
	m_pMOG2 = cv::createBackgroundSubtractorMOG2(); //MOG2 approach
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTbgs::~TGMTbgs()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTbgs::Init(BgsType type)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTbgs::Process(cv::Mat matInput)
{
	cv::Mat fgMaskMOG2;

	m_pMOG2->apply(matInput, fgMaskMOG2);

	if (m_blurSize > 0 && m_blurSize % 2 == 1)
	{
		//cv::blur(matInput, matInput, cv::Size(m_blurSize, m_blurSize));

		cv::GaussianBlur(fgMaskMOG2, fgMaskMOG2, cv::Size(m_blurSize, m_blurSize),0);
		//fgMaskMOG2 = TGMTbrightness::EqualizeHist(fgMaskMOG2);
		cv::threshold(fgMaskMOG2, fgMaskMOG2, 200, 255, cv::THRESH_BINARY);

		int morph_size = 17;
		//cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2 * morph_size + 1, 2 * morph_size + 1), cv::Point(morph_size, morph_size));

		/// Apply the specified morphology operation
		//cv::morphologyEx(fgMaskMOG2, fgMaskMOG2, cv::MORPH_CLOSE, element);
		fgMaskMOG2 = TGMTmorphology::Dilate(fgMaskMOG2, cv::MORPH_RECT, 13);
		fgMaskMOG2 = TGMTmorphology::Erode(fgMaskMOG2, cv::MORPH_RECT, 7);
	}

	imshow("FG Mask MOG 2", fgMaskMOG2);
	imshow("output", matInput);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MANAGED

void TGMTbgsBridge::Init(int type)
{
	GetTGMTbgs()->Init((TGMTbgs::BgsType)type);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTbgsBridge::Process(Bitmap^ bmp)
{
	cv::Mat mat = TGMTbridge::BitmapToMat(bmp);
	GetTGMTbgs()->Process(mat);
	cv::waitKey(1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//void TGMTbgsBridge::Process(System::String^ videoPath)
//{
//
//}
#endif


bool TGMTbgs::SetBlurSize(int size)
{
	if (size < 1 || size % 2 == 0)
		return false;
	m_blurSize = size;
}