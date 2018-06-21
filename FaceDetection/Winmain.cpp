#include "stdafx.h"
#include <tchar.h>
#include "TGMTfile.h"
#include "TGMTdebugger.h"
#include "TGMTutil.h"
#include <windows.h>
#include "TGMTConfig.h"
#include "FaceDetection.h"
#include "TGMTface.h"
#include "TGMTdraw.h"
#include "TGMTcamera.h"

#define INI "FaceDetection"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OnNewFrame(cv::Mat frame)
{
	int count = GetTGMTface()->DetectAndDrawFaces(frame);
	TGMTdraw::PutText(frame, cv::Point(10, 30), GREEN, "%d face", count);
	ShowImage(frame, "https://thigiacmaytinh.com - Face detection");
	cv::waitKey(1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
	SET_CONSOLE_TITLE("https://thigiacmaytinh.com - Face detection");

	
	ASSERT(GetTGMTConfig()->LoadSettingFromFile(), "Can not load file config.ini");
	GetTGMTface()->Init();

	int source = GetTGMTConfig()->ReadValueInt(INI, "source", 1);
	if (source == 0)//image
	{
		std::string imgPath = GetTGMTConfig()->ReadValueString(INI, "image");
		cv::Mat mat = cv::imread(imgPath);
		ASSERT(mat.data, "Can not load image");

		int count = GetTGMTface()->DetectAndDrawFaces(mat);
		ShowImage(mat, "Detected %d faces", count);
	}
	else if (source == 1)//camera
	{
		GetTGMTcamera()->OnNewFrame = OnNewFrame;
		ASSERT(GetTGMTcamera()->LoadConfig(), "Can not load camera");

		
		GetTGMTcamera()->Start();
	}
	else // video
	{

	}
	
	cv::waitKey();
	getchar();

	return 0;
}