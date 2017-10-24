// FaceRecognition.cpp : Defines the entry point for the console application.
//

#ifndef LIB
#include "stdafx.h"
#include <tchar.h>
#include <time.h>
#include "TGMTfile.h"
#include "TGMTdebugger.h"
#include "TGMTutil.h"
#include <ppl.h>
#include <windows.h>
#include "TGMTConfig.h"
#include "FaceComp.h"
#include "TGMTface.h"
#include "TGMTdraw.h"
#include "TGMTtransform.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PrintHelp()
{
	PrintMessage("Use with some parameters:");

	PrintMessageGreen("FaceComp.exe -createsample <image path|dir> <output dir>");
#ifdef _DEBUG
	PrintMessageGreen("             -recog <image path|dir> [-cropped] [-debug]");
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateData()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
	SET_CONSOLE_TITLE("FaceCom - Face Compare");
	if (argc < 2)
	{
		PrintHelp();
	}
	else
	{
		std::string cmd = argv[1];

		bool cropped = TGMTutil::CheckParameterAloneExist(argc, argv, "-cropped");
		bool enableDebug = TGMTutil::CheckParameterAloneExist(argc, argv, "-debug");

		GetTGMTConfig()->LoadSettingFromFile("FaceComp_config.ini");

#if !defined(LITE)		
		if (cmd == "-detect")
		{
			GetTGMTface()->Init();
			std::string inputPath = argv[2];
			cv::Mat matInput = cv::imread(inputPath);
			std::vector<cv::Rect> faces = GetTGMTface()->DetectFaces(matInput);
			PrintMessage("Detected %d face(s)", faces.size());

			if (enableDebug)
			{
				TGMTdraw::DrawRectangles(matInput, faces, matInput.cols > 800 ? 2 : 1, GREEN);
				
				matInput = TGMTtransform::ResizeByScreen(matInput, 100);
				
				if(faces.size() > 0)
					cv::imshow("Face detected", matInput);
				else
					cv::imshow("Can not detect faces", matInput);
				cv::waitKey(0);
			}
		}
#endif
		if (cmd == "-recog")
		{
			std::string inputPath = argv[2];
			GetFaceComp()->RecogSingleImage(inputPath, cropped);
			GetFaceComp()->m_enableDebug = enableDebug;
		}
		else if (cmd == "-createsample")
		{
			std::string inputPath = argv[2];
			std::string outputDir = argv[3];
			std::vector<std::string> files;

			if (TGMTfile::IsDir(inputPath))
			{
				files = TGMTfile::GetImageFilesInDir(inputPath);
				TGMTfile::CorrectPath(inputPath);
				TGMTfile::CorrectPath(outputDir);
				TGMTfile::CreateDir(outputDir);
			}
			else if (TGMTfile::IsImage(inputPath))
			{
				files.push_back(inputPath);
			}
			else
			{
				PrintError("Input is not valid");
				return 0;
			}

			GetTGMTface()->Init();
			
			float expandRatio = GetTGMTConfig()->ReadValueFloat(INI_FACE_SECTION, "expand_face_rect_ratio", 1.f);
			int faceCount = 0;
			int facewidth = GetTGMTConfig()->ReadValueInt(INI_FACE_SECTION, "face_size", 100);
			cv::Size faceSize = cv::Size(facewidth, facewidth);

			for (int i = 0; i < files.size(); i++)
			{
				cv::Mat matGray = cv::imread(files[i], CV_LOAD_IMAGE_GRAYSCALE);
				std::vector<cv::Rect> rects = GetTGMTface()->DetectFaces(matGray);
				if (rects.size() == 0)
				{
					PrintError("%s: can not detect face", files[i].c_str());
					continue;
				}

				SET_CONSOLE_TITLE("%d / %d", i, files.size());
				std::string outputFile = outputDir + TGMTfile::GetFileNameWithoutExtension(TGMTfile::GetFileName(files[i])) + ".jpg";
				for (int j = 0; j < rects.size(); j++)
				{
					PrintMessageYellow("%s: %d faces", files[i].c_str(), rects.size());
					faceCount += rects.size();
					cv::Rect rectExpanded = TGMTshape::ExpandRect(rects[j], expandRatio, expandRatio);
					cv::Mat matFace;


					if (TGMTshape::IsRectInsideMat(rectExpanded, matGray))
					{
						matFace = matGray(rectExpanded);
					}
					else
					{
						matFace = matGray(rects[j]);
					}
					if (matFace.cols != facewidth && matFace.rows != facewidth)
					{
						cv::resize(matFace, matFace, faceSize);
					}					
					cv::imwrite(outputFile, matFace);
				}
			}
			if (faceCount == 0)
				PrintMessage("Can not detect any face in %d images", files.size());
			else
				PrintSuccess("Detected %d faces in %d images", faceCount, files.size());
		}
		else
		{
			PrintHelp();
		}

	}
#ifdef _DEBUG
	getchar();
#endif
	return 0;
}
#endif