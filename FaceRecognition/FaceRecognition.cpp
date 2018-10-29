#include "FaceRecognition.h"
#include "TGMTConfig.h"
#include "TGMTface.h"
#include "TGMTutil.h"
#include "TGMTdebugger.h"
#include "TGMTfile.h"
#include "TGMTshape.h"
#include "TGMTface.h"
#include "json\json.h"
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

std::vector<std::string> FaceRecognition::RecogImage(cv::Mat matInput, bool cropped)
{
	std::vector<std::string> nameList;
	if (!matInput.data)
	{
		return nameList;
	}
	std::vector<cv::Rect> rects;
	std::string errMsg = "";
	std::vector<TGMTface::Person> persons = GetTGMTface()->DetectPersons(matInput, rects, errMsg, cropped);

	for (int i = 0; i < persons.size(); i++)
	{
		nameList.push_back(persons[i].name);
	}
	if (m_enableDebug)
	{
		cv::imshow("Debug window", matInput);
		cv::waitKey();
	}

	return nameList;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string FaceRecognition::RecogImages(std::string inputPath, std::string &errMsg, bool cropped)

{
	std::vector<std::pair< std::string, std::string>> result;
	if (TGMTfile::IsImage(inputPath))
	{
		cv::Mat mat = cv::imread(inputPath);
		if (!mat.data)
		{
			PrintError("Can not read image: %s", inputPath.c_str());
		}
		std::string name = TGMTutil::JoinVectorString(RecogImage(mat, cropped));

		result.push_back(std::pair< std::string, std::string>(inputPath, name));
	}
	else if (TGMTfile::IsDir(inputPath))
	{
		std::vector<std::string> files = TGMTfile::GetFilesInDir(inputPath);
		for (int i = 0; i < files.size(); i++)
		{
			cv::Mat mat = cv::imread(files[i]);
			std::vector<std::string> names = RecogImage(mat, cropped);
			std::string name = TGMTutil::JoinVectorString(names);

			result.push_back(std::pair< std::string, std::string>(files[i], name == ""? "Can't detect" : name));
		}
		
	}
	else
	{
		errMsg = "Input is invalid";
#ifdef _DEBUG
		PrintError(errMsg.c_str());
#endif
	}

	return PrintOutputJson(result,errMsg) ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string FaceRecognition::PrintOutputJson(std::vector<std::pair< std::string, std::string>> result, std::string errMsg)
{
	Json::Value json;

	if (result.size() == 0)
	{
		json["person"] = Json::nullValue;
	}
	if (result.size() == 1)
	{
		json["person"] = result[0].second;
	}
	else
	{
		for (int i = 0; i < result.size(); i++)
		{
			json["person"].append(result[i].second);

		}
	}
	json["error"] = errMsg;

	std::cout << json;
	return json.asCString();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string FaceRecognition::RecogSingleImage(cv::Mat matInput, bool cropped)
{
	Json::Value json;
	json["person"] = "";
	json["confident"] = 0;
	json["error"] = "";

	Json::FastWriter fastWriter;
	if (!matInput.data)
	{
		json["error"] = "Can not load image";
		return fastWriter.write(json);
	}
	
	std::vector<cv::Rect> rects;
	std::string errMsg;
	std::vector<TGMTface::Person> persons = GetTGMTface()->DetectPersons(matInput, rects, errMsg, cropped);
	if (persons.size() == 0)
	{
		json["error"] = errMsg;
	}
	else
	{
		if (persons[0].confident > 0 && persons[0].confident < m_confident)
		{
			json["person"] = persons[0].name;
			json["confident"] = persons[0].confident;			
		}
		else
		{
			json["error"] = TGMTutil::FormatString("Can not find person with confident %d", m_confident);
		}
	}
	
	std::cout << json;	
	return fastWriter.write(json);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string FaceRecognition::RecogSingleImage(std::string imgPath, bool cropped)
{
	if (TGMTfile::FileExist(TGMTfile::GetCurrentDir() + "\\" + imgPath))
	{
		PrintError("File %s\\%s does not exist", TGMTfile::GetCurrentDir().c_str(),  imgPath.c_str());
		return "";
	}
	return RecogSingleImage(cv::imread(imgPath), cropped);
}