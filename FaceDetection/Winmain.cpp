// FaceRecognition.cpp : Defines the entry point for the console application.
//

#ifndef LIB_CS
#include "stdafx.h"
#include <tchar.h>
#include <time.h>
#include "TGMTfile.h"
#include "TGMTdebugger.h"
#include "TGMTutil.h"
#include <ppl.h>
#include <windows.h>
#include "TGMTConfig.h"
#include "FaceDetection.h"
#include "TGMTface.h"
#include "TGMTdraw.h"

#define INI "FaceDetection"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
	SET_CONSOLE_TITLE("https://thigiacmaytinh.com - Face detection");

	
	ASSERT(GetTGMTConfig()->LoadSettingFromFile(), "Can not load file config.ini");
	GetTGMTface()->Init();

	int source = GetTGMTConfig()->ReadValueInt(INI, "source", 1);
	if (source == 0)
	{
		std::string imgPath = GetTGMTConfig()->ReadValueString(INI, "image");
		cv::Mat mat = cv::imread(imgPath);
		ASSERT(mat.data, "Can not load image");

		int count = GetTGMTface()->DetectAndDrawFaces(mat);
		ShowImage(mat, "Detected %d faces", count);
	}
	else if (source == 1)
	{

	}
	else
	{

	}
	
	cv::waitKey();
	getchar();

	return 0;
}
#endif