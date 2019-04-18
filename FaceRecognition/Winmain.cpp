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
//#include "FaceRecognition.h"
#include "TGMTface.h"
#include "TGMTdraw.h"
#include "TGMTcamera.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int g_confident;
int g_id = 1;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PrintHelp()
{
	PrintMessage("Use with some parameters:");

	PrintMessageGreen("FaceRecognition.exe -createsample <image path|dir> <output dir>");
#ifdef _DEBUG
	PrintMessageGreen("             -recog <image path|dir> [-cropped] [-debug]");
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LoadConfig()
{
	GetTGMTConfig()->LoadSettingFromFile();
	GetTGMTface()->Init();
	
	std::string strConfident = GetTGMTConfig()->ReadValueString(INI_TGMTFACE_SECTION, "guest_confident", "LBPH_confident");
	g_confident = GetTGMTConfig()->ReadValueInt(INI_TGMTFACE_SECTION, strConfident, -1);

	GetTGMTface()->Training();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OnCameraFrame(std::vector<cv::Mat> frames)
{
	if (frames.size() == 0)
	{
		PrintError("Can not get image from camera");
		return;
	}

	std::vector<cv::Rect> rects;
	std::string errMsg;
	for (int i = 0; i < frames.size(); i++)
	{
		cv::Mat frame = frames[i];
		std::vector<TGMTface::Person> persons = GetTGMTface()->DetectPersons(frame, rects, errMsg);

		TGMTdraw::DrawRectangles(frame, rects, 1, GREEN);

		for (int j = 0; j < persons.size(); j++)
		{
			cv::Point p = rects[j].tl();
			p.y -= 20;
			TGMTdraw::PutText(frame, p, GREEN, "%s", persons[i].name.c_str());

			if (persons[i].confident < g_confident)
			{
				WriteImageAsync(frame(rects[j]), "faces\\%s\\%s.png",   GetCurrentDateTime(true).c_str());
			}
		}
		

		ShowImage(frames[i], "camera_%d", i);
	}
	cv::waitKey(1);
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
	SET_CONSOLE_TITLE("FaceRecognition - Recognize face");

	LoadConfig();

	GetTGMTcamera()->OnNewFrames = OnCameraFrame;
	GetTGMTcamera()->LoadConfig();
	GetTGMTcamera()->Start();

#ifdef _DEBUG
	getchar();
#endif
	return 0;
}
#endif