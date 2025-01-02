#include "FaceRecognition.h"
#include "TGMTConfig.h"
#include "TGMTface.h"
#include "TGMTutil.h"
#include "TGMTdebugger.h"
#include "TGMTfile.h"
#include "TGMTshape.h"
#include "TGMTface.h"
#include <mutex>

std::mutex m_mutex;
bool g_isRuning = true;

FaceRecognition* FaceRecognition::m_instance = nullptr;
cv::Mat m_mat;


int key = -1;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FaceRecognition::FaceRecognition()
{
#ifdef LIB_CS
	GetTGMTConfig()->LoadSettingFromFile("FaceComp_config.ini");
#endif
	GetTGMTface()->Init();
	
	std::string strConfident = GetTGMTConfig()->ReadValueString(INI_TGMTFACE_SECTION, "guest_confident", "LBPH_confident");
	m_confident = GetTGMTConfig()->ReadValueInt(INI_TGMTFACE_SECTION, strConfident, -1);

	GetTGMTface()->Training();

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FaceRecognition::~FaceRecognition()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string FaceRecognition::RecogImages(std::string inputPath, std::string &errMsg, bool cropped)

{
//	std::vector<std::pair< std::string, std::string>> result;
//	if (TGMTfile::IsImage(inputPath))
//	{
//		cv::Mat mat = cv::imread(inputPath);
//		if (!mat.data)
//		{
//			PrintError("Can not read image: %s", inputPath.c_str());
//		}
//		std::string name = TGMTutil::JoinVectorString(RecogImage(mat, cropped));
//
//		result.push_back(std::pair< std::string, std::string>(inputPath, name));
//	}
//	else if (TGMTfile::IsDir(inputPath))
//	{
//		std::vector<std::string> files = TGMTfile::GetFilesInDir(inputPath);
//		for (int i = 0; i < files.size(); i++)
//		{
//			cv::Mat mat = cv::imread(files[i]);
//			std::vector<std::string> names = RecogImage(mat, cropped);
//			std::string name = TGMTutil::JoinVectorString(names);
//
//			result.push_back(std::pair< std::string, std::string>(files[i], name == ""? "Can't detect" : name));
//		}
//		
//	}
//	else
//	{
//		errMsg = "Input is invalid";
//#ifdef _DEBUG
//		PrintError(errMsg.c_str());
//#endif
//	}
	return "";
}
