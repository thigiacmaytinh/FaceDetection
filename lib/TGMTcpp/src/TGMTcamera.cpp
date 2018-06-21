#include "TGMTcamera.h"
#include "TGMTdebugger.h"
#ifdef WIN32
#include <Windows.h>
#endif
#include <mutex> 
#include "TGMTfile.h"
#include "TGMTdraw.h"
#include "TGMTutil.h"
#include "TGMTConfig.h"
#ifdef OS_LINUX
#include <unistd.h>
#endif
TGMTcamera* TGMTcamera::instance = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTcamera::TGMTcamera()
{
	m_isRunning = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTcamera::~TGMTcamera()
{
	mCap.~VideoCapture();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTcamera::PlayCamera(int cameraID, int delayMilisecond)
{
	// open the camera
	cv::VideoCapture cap = cv::VideoCapture(cameraID);
	std::string windowName = TGMTutil::FormatString("Camera %d", cameraID);
	if (cap.isOpened())  // check if we succeeded
	{
		cv::Mat frame;
		while (true)
		{
			cap >> frame;

			cv::imshow(windowName, frame);
			#ifndef OS_LINUX
			if (cv::waitKey(delayMilisecond) == VK_ESCAPE) break;
			#endif
		}
	}
	else
	{
		PrintError("Not found any camera");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat TGMTcamera::GetImageFromCamera(int cameraID)
{
	if (!mCap.isOpened())
	{
		mCap = cv::VideoCapture(cameraID);
		ASSERT(mCap.isOpened(), "Can not get image from webcam");
	}
	cv::Mat frame;

	mCap >> frame;
	return frame;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTcamera::StartCamera(int cameraID)
{
	ASSERT(OnNewFrame, "You must set callback function before use this function");

	StopCamera();
	mCap = cv::VideoCapture(cameraID);
	ASSERT(mCap.isOpened(), "Can not read camera %d", cameraID);

	m_isRunning = true;
	m_threadGrab = std::thread(&TGMTcamera::CameraGrabbed, this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTcamera::StartCamera(std::string cameraURL)
{
	ASSERT(OnNewFrame, "You must set callback function before use this function");

	StopCamera();
	mCap = cv::VideoCapture(cameraURL);
	ASSERT(mCap.isOpened(), "Can not read camera %s", cameraURL.c_str());

	m_isRunning = true;
	m_threadGrab = std::thread(&TGMTcamera::CameraGrabbed, this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTcamera::StopCamera()
{
	if (m_isRunning)
	{
		m_isRunning = false;
		if (m_threadGrab.joinable())
		{
			m_threadGrab.join();
		}


		if (mCap.isOpened())
		{
			mCap.release();
		}

		for (int i = 0; i < m_threads.size(); i++)
		{
			m_threads[i]->join();
		}

		for (int i = 0; i < m_vcaps.size(); i++)
		{
			if (m_vcaps[i]->isOpened())
			{
				m_vcaps[i]->release();
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTcamera::CameraGrabbed()
{
	cv::Mat frame;

	while (mCap.isOpened() && m_isRunning)
	{
		mCap.read(frame);

		OnNewFrame(frame);
	}
	frame.~Mat();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTcamera::CaptureFrame(int camID)
{
	cv::Mat frame;
	cv::VideoCapture* cam = m_vcaps[camID];
	while (m_isRunning && cam->isOpened())
	{
		cam->read(frame);

		if (m_frameResizes[camID].area() > 0 && frame.data && frame.cols != m_frameResizes[camID].width && frame.rows != m_frameResizes[camID].height)
		{
			cv::resize(frame, frame, m_frameResizes[camID]);
		}

		m_mutex.lock();
		frame.copyTo(m_frames[camID]);
		m_mutex.unlock();
	}
	frame.~Mat();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTcamera::RecordVideo(int cameraID, std::string videoFileOutputPath)
{
	TODO("Not implemented yet");
	cv::VideoCapture inputVideo(cameraID);
	if (!inputVideo.isOpened())
	{
		PrintError("Could not open camera %d", cameraID);
		return;
	}

	cv::VideoWriter outputVideo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTcamera::StartCameras(std::vector<std::string> camSources)
{
	ASSERT(camSources.size() > 0, "Number camera support must bigger than 0");
	ASSERT(OnNewFrames != nullptr, "You must set callback function before call StartCameras");


	//check valid cam
	for (int i = 0; i < camSources.size(); i++)
	{
		if (TGMTutil::IsNumber(camSources[i]))
		{
			if (IsValidCamera(atoi(camSources[i].c_str())))
			{
				m_vcaps.push_back(new cv::VideoCapture(atoi(camSources[i].c_str())));
			}
			else
			{
				PrintError("Webcam %s load failed", camSources[i].c_str());
				WriteLog("Webcam %s load failed", camSources[i].c_str());
			}
		}
		else
		{
			if (IsValidCamera(camSources[i]))
			{
				m_vcaps.push_back(new cv::VideoCapture(camSources[i].c_str(), cv::CAP_FFMPEG));
			}
			else
			{
				PrintError("Camera link load failed: %s", camSources[i].c_str());
				WriteLog("Camera link load failed: %s", camSources[i].c_str());
			}
		}
	}
	if (m_vcaps.size() == 0)
	{
		PrintError("Can not load any camera");
		WriteLog("Can not load any camera");
		m_isRunning = false;
		return;
	}


	m_frames = std::vector<cv::Mat>(m_vcaps.size());

	//get frames size
	for (int i = 0; i < m_vcaps.size(); i++)
	{
		m_vcaps[i]->read(m_frames[i]);
		m_frameOriginSizes.push_back(cv::Rect(0, 0, m_frames[i].cols, m_frames[i].rows));

		std::thread* t = new std::thread(&TGMTcamera::CaptureFrame, this, i);
		m_threads.push_back(t);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTcamera::Start()
{
	if (!m_loadConfigSuccess)
	{
		PrintError("Load camera config failed");
		return;
	}
	if (m_camSources.size() == 0)
	{
		PrintError("Num of valid camera is zero");
		return;
	}

	m_isRunning = true;

	if (m_camSources.size() == 1)
	{
		if (TGMTutil::IsNumber(m_camSources[0]))
			StartCamera(atoi(m_camSources[0].c_str()));
		else
			StartCamera(m_camSources[0].c_str());
	}
	else
	{
		StartCameras(m_camSources);
	}
	m_threadUpdate = std::thread(&TGMTcamera::Update, this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTcamera::LoadConfig()
{
	PrintMessage("Loading camera in config.ini");
	m_loadConfigSuccess = false;
	//read config
	int numCamera = GetTGMTConfig()->ReadValueInt("TGMTcamera", "num_camera");
	if (numCamera == 0)
	{
		PrintError("Num of camera is 0");
		WriteLog("Num of camera is 0");
		return false;
	}


	for (int i = 0; i < numCamera; i++)
	{
		std::string camKey = TGMTutil::FormatString("camera_%d", i);
		std::string camSource = GetTGMTConfig()->ReadValueString("TGMTcamera", camKey);
		if (camSource.empty())
		{
			PrintError("Can not load camera config: %s", camKey.c_str());
			WriteLog("Can not load camera config: %s", camKey.c_str());
			continue;
		}

		if (camSource.find("\n") != -1)
		{
			PrintError("%s is duplicated", camKey.c_str());
			WriteLog("%s is duplicated", camKey.c_str());
			continue;
		}

		std::string camResizeKey = TGMTutil::FormatString("camera_%d_resize", i);
		auto strResizeSplit = TGMTutil::SplitString(GetTGMTConfig()->ReadValueString("TGMTcamera", camResizeKey), ',');
		if (strResizeSplit.size() != 2)
		{
			m_frameResizes.push_back(cv::Size(0, 0));
		}
		else
		{
			m_frameResizes.push_back(cv::Size(atoi(strResizeSplit[0].c_str()), atoi(strResizeSplit[1].c_str())));
		}
		m_camSources.push_back(camSource);
	}


	if (m_camSources.size() > 0)
	{
		m_loadConfigSuccess = true;
		PrintSuccess("Load %d camera config success", m_camSources.size());
		WriteLog("Load %d camera config success", m_camSources.size());
		return true;
	}
	else
	{
		PrintError("Can not load any camera config");
		WriteLog("Can not load any camera config");
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int TGMTcamera::GetNumOfCamera()
{
	return m_camSources.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTcamera::IsValidCamera(int camSource)
{
	cv::VideoCapture cap = cv::VideoCapture(camSource);
	bool valid = cap.isOpened();
	cap.release();
	return valid;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTcamera::IsValidCamera(std::string camLink)
{
	cv::VideoCapture cap = cv::VideoCapture(camLink);
	bool valid = cap.isOpened();
	cap.release();
	return valid;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Rect TGMTcamera::GetFrameSize(int camSource)
{
	if (camSource < 0 || m_frameOriginSizes.size() == 0 || camSource > m_frameOriginSizes.size())
		return cv::Rect();

	return m_frameOriginSizes[camSource];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTcamera::Update()
{
	while (m_isRunning)
	{
		if (m_frames.size() == 0)
		{
			#ifdef OS_LINUX
				usleep(1000);
			#else
				Sleep(100);
			#endif
			continue;
		}
		OnNewFrames(m_frames);
	}
	if (OnStopped != nullptr)
		OnStopped();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTcamera::GetStoredFrames(std::vector<cv::Mat>& frames)
{
	m_mutex.lock();
	for (size_t i = 0; i < m_frames.size(); i++)
	{
		frames.push_back(m_frames[i].clone());
	}
	m_mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTcamera::IsCameraRunning()
{
	return m_isRunning;
}