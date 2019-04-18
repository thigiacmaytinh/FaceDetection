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
#include "TGMTvideo.h"
#include "TGMTface.h"
#include "TGMTdraw.h"
#include "TGMTcamera.h"

#define INI "FaceDetection"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int g_confident;
int g_id = 1;
int g_skip_n_frame;
int g_frameCount = 0;
std::vector<TGMTface::Person> g_persons;
std::vector<cv::Rect> g_rects;

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
	g_skip_n_frame = GetTGMTConfig()->ReadValueInt(INI, "skip_n_frame", 10);

	GetTGMTface()->Training();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OnNewVideoFrame(cv::Mat frame)
{
	
	std::string errMsg;
	if (g_frameCount < 10)
	{
		g_frameCount++;
	}
	else
	{
		g_frameCount = 0;
		g_persons = GetTGMTface()->DetectPersons(frame, g_rects, errMsg);
	}
	
	if (g_persons.size() > 0)
	{
		TGMTdraw::DrawRectangles(frame, g_rects, 1, GREEN);

		for (int j = 0; j < g_persons.size(); j++)
		{
			cv::Point p = g_rects[j].tl();
			p.y -= 20;
			TGMTdraw::PutText(frame, p, GREEN, "%s", g_persons[j].name.c_str());

		}
	}

	
	ShowImage(frame, "https://thigiacmaytinh.com - Face recognition");
	cv::waitKey(1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OnCameraFrames(std::vector<cv::Mat> frames)
{
	if (frames.size() == 0)
	{
		PrintError("Can not get image from camera");
		return;
	}

	
	for (int i = 0; i < frames.size(); i++)
	{
		cv::Mat frame = frames[i];
		
		OnNewVideoFrame(frame);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
	SET_CONSOLE_TITLE("FaceRecognition - Recognize face");

	LoadConfig();

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
		GetTGMTcamera()->OnNewFrames = OnCameraFrames;
		ASSERT(GetTGMTcamera()->LoadConfig(), "Can not load camera");


		GetTGMTcamera()->Start();
	}
	else // video
	{
		GetTGMTvideo()->OnNewFrame = OnNewVideoFrame;
		GetTGMTvideo()->LoadConfig();
		GetTGMTvideo()->PlayVideo();
	}


#ifdef _DEBUG
	getchar();
#endif
	return 0;
}
#endif