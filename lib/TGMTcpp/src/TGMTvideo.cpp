#include "TGMTvideo.h"
#include "TGMTdebugger.h"
#include "TGMTfile.h"
#ifdef WIN32
#include <Windows.h>
#endif
#include "TGMTutil.h"


TGMTvideo* TGMTvideo::instance = NULL;

std::string m_outDir;


TGMTvideo::TGMTvideo()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTvideo::~TGMTvideo()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTvideo::LoadVideoFile(std::string videoFilePath)
{

	ASSERT(TGMTfile::FileExist(videoFilePath), "File %s does not exist", videoFilePath.c_str());
#ifdef WIN32
	ASSERT(TGMTfile::FileExist(TGMTfile::GetCurrentDir() + "\\opencv_ffmpeg320.dll"), "File opencv_ffmpeg320.dll not found");
#endif
	mCap = cv::VideoCapture(videoFilePath);
	if (!mCap.isOpened())
	{		
		PrintError("Can not load video %s", videoFilePath.c_str());

		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTvideo::PlayVideo(std::string videoFilePath, cv::Size maxSize, int fps, int startIndex, int total)
{
	ASSERT(OnNewFrame, "You must set callback function before use this function");
	
	if (!LoadVideoFile(videoFilePath))
		return;
	m_stopVideo = false;

	uint amount = mCap.get(CV_CAP_PROP_FRAME_COUNT);
	ASSERT(startIndex >= 0 && startIndex < amount, "start index (%d) must less than amount frame (%d)", startIndex, amount);

	if (startIndex != 0 && startIndex > 0 && startIndex < amount)
	{
		mCap.set(CV_CAP_PROP_POS_FRAMES, startIndex);
	}
	cv::Mat frame;
	int duration = 0;
	if (fps != 0)
	{
		duration = 1000 / fps;
	}

	m_frameCount = startIndex;
	int key = -1;
#ifdef WIN32
	while (key != VK_ESCAPE && !m_stopVideo)
#else //TODO: need to check which value (keycode) is return for orther GUI (QT, GTK ..)
	while (!m_stopVideo)
#endif
	{
		START_COUNT_TIME("playvideo");
		mCap.read(frame);
		if (!frame.data)
		{
			break;
		}
		if ((maxSize.width != 0 && maxSize.height != 0) && (frame.cols > maxSize.width || frame.rows > maxSize.height))
		{
			cv::resize(frame, frame, maxSize);
		}
		OnNewFrame(frame);

		if (fps != 0)
		{
			int elapsedTime = STOP_COUNT_TIME("playvideo");
			if (elapsedTime < duration)
			{
				key = cv::waitKey(duration - elapsedTime);
			}
		}

		m_frameCount++;
		if (total > 0 && m_frameCount - startIndex >= total )
		{
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SaveFrame(cv::Mat frame)
{
	WRITE_IMAGE_ASYNC(frame, "%s\\frame_%d.jpg", m_outDir.c_str(), GetTGMTvideo()->m_frameCount);
	std::cout << "\r" << "Writing frame " << GetTGMTvideo()->m_frameCount;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTvideo::ExtractFrame(std::string videoFilePath, std::string outputDir, cv::Size outputSize, int startIndex, int total)
{
	PrintMessage("Total frame: %d", GetAmountFrame(videoFilePath));
	OnNewFrame = SaveFrame;
	m_outDir = outputDir;	
	TGMTfile::CreateDir(outputDir);	
	PlayVideo(videoFilePath, outputSize, 0, startIndex, total);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat TGMTvideo::GetFrameAt(std::string videoFilePath, int frameIndex)
{
	//in function GetAmountFrame() has loaded video
	ASSERT(0 < frameIndex && frameIndex < GetAmountFrame(videoFilePath));

	mCap.set(CV_CAP_PROP_POS_FRAMES, frameIndex);
	cv::Mat frame;
	mCap.read(frame);
	return frame;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

int TGMTvideo::GetAmountFrame(std::string videoFilePath)
{
	if (!LoadVideoFile(videoFilePath))
		return -1;

	int amountOfFrame = mCap.get(CV_CAP_PROP_FRAME_COUNT);
	return amountOfFrame;
}

