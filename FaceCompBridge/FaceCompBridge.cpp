// This is the main DLL file.


#include "FaceComp.h"
#include "FaceCompBridge.h"
#include "TGMTbridge.h"

//using namespace FaceCompBridgeLib;

FaceCompBridge::FaceCompBridge()
{
	GetFaceComp();
}

FaceCompBridge::~FaceCompBridge()
{

}

String^ FaceCompBridge::RecogImage(System::Drawing::Bitmap^ bmp, bool cropped)
{
	cv::Mat mat = TGMT::TGMTbridge::BitmapToMat(bmp);
	std::string result = GetFaceComp()->RecogSingleImage(mat, cropped);
	return TGMT::TGMTbridge::stdStrToSystemStr(result);
}

String^ FaceCompBridge::RecogImage(String^ imgPath, bool cropped)
{
	std::string imagePath = TGMT::TGMTbridge::SystemStr2stdStr(imgPath);
	std::string result = GetFaceComp()->RecogSingleImage(imagePath, cropped);
	return TGMT::TGMTbridge::stdStrToSystemStr(result);
}