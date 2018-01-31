#include "TGMTml.h"
#include "TGMTdebugger.h"
#include "TGMTfile.h"
#include "TGMTutil.h"
#include "TGMTimage.h"

#if defined(WIN32) || defined(WIN64)
#include "TGMTConfig.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTml::TGMTml()
{
#if defined(WIN32) || defined(WIN64)
	std::string desireSize = GetTGMTConfig()->ReadValueString("TGMTml", "desire_size");
	auto splitDesireSize = TGMTutil::SplitString(desireSize, ',');
	if (splitDesireSize.size() == 2)
	{
		m_desireSize = cv::Size(Str2Int(splitDesireSize[0]), Str2Int(splitDesireSize[1]));
	}

	int type = GetTGMTConfig()->ReadValueInt("TGMTml", "data_type");
	if (type >= 0 && type < sizeof(DataType))
	{
		m_dataType = (DataType)type;
	}
	else
	{
		PrintError("DataType is invalid");
	}


	m_enableThreshold = GetTGMTConfig()->ReadValueBool("TGMTml", "enable_threshold");
	if (m_enableThreshold)
	{
		m_thresholdValue = GetTGMTConfig()->ReadValueInt("TGMTml", "threshold_value");
		if (m_thresholdValue <= 0 || m_thresholdValue > 255)
		{
			m_enableThreshold = false;
		}
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTml::~TGMTml()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTml::TrainData(std::string dirPath)
{
	std::vector<std::string> files = TGMTfile::GetImageFilesInDir(dirPath, true);
	if (files.size() == 0)
	{
		PrintError("Not found any image");
		return false;
	}

	std::vector<std::string> validFiles;
	std::vector<int> validLabels;
	for (int i = 0; i < files.size(); i++)
	{
		std::string filePath = files[i];
		std::string parentDir = TGMTfile::GetParentDir(filePath, false);
		if (parentDir.length() > 2)
			continue;

		validFiles.push_back(filePath);
		int label = (char)parentDir[0];
		validLabels.push_back(label);
	}

	return TrainData(validFiles, validLabels);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTml::TrainData(std::vector<std::string> imgPaths, std::vector<int> labels)
{
	if (imgPaths.size() == 0 || labels.size() == 0)
	{
		PrintError("Data input is empty");
		return false;
	}
	if (imgPaths.size() != labels.size())
	{
		PrintError("Image set not equal label set");
		return false;
	}
		
	size_t numMats = imgPaths.size();

	int matArea =  m_desireSize.width * m_desireSize.height;
	m_matData = cv::Mat(numMats, matArea, CV_32FC1);
	m_matLabel = cv::Mat(numMats, 1, CV_32SC1);

	//prepare train set
	for (size_t fileIndex = 0; fileIndex < numMats; fileIndex++)
	{

		//set label
		m_matLabel.at<int>(fileIndex, 0) = labels[fileIndex];

		cv::Mat mat = cv::imread(imgPaths[fileIndex], CV_LOAD_IMAGE_GRAYSCALE);
		mat = PrepareMatData(mat);
		
		mat.row(0).copyTo(m_matData.row(fileIndex));
	}


	return TrainData(m_matData, m_matLabel);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTml::SaveMatData(std::string fileName)
{
	cv::FileStorage file(fileName, cv::FileStorage::WRITE);
	file << "traindata" << m_matData;
	file << "trainlabel" << m_matLabel;
	file.release();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTml::LoadData(std::string fileName)
{
	if (!TGMTfile::FileExist(fileName))
	{
		PrintError("File does not exist: %s", fileName.c_str());
		return false;
	}

	switch (m_dataType)
	{
	case DataType::raw:
		return LoadMatData(fileName);
	case DataType::trained:
		return LoadModel(fileName);
	};
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTml::SaveData(std::string fileName)
{
	switch (m_dataType)
	{
	case DataType::raw:
		SaveMatData(fileName);
		break;
	case DataType::trained:
		SaveModel(fileName);
		break;
	};
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTml::LoadMatData(std::string trainDataFile)
{
	cv::FileStorage fs;
	fs.open(trainDataFile, cv::FileStorage::READ);
	ASSERT(fs.isOpened(), "Can not load %s", trainDataFile.c_str());
	cv::Mat matData;
	cv::Mat matClass;
	fs["traindata"] >> matData;
	fs["trainlabel"] >> matClass;
	fs.release();

	if (!matData.data)
	{
		PrintError("Mat data is empty");
		return false;
	}
	if (!matClass.data)
	{
		PrintError("Mat label is empty");
		return false;
	}

	return TrainData(matData, matClass);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat TGMTml::PrepareMatData(cv::Mat matInput)
{
	ASSERT(matInput.data, "Mat input is null");
	cv::Mat mat = matInput.clone();
	mat = TGMTimage::ConvertToGray(mat);

	if (mat.cols != m_desireSize.width || mat.rows != m_desireSize.height)
	{
		cv::resize(mat, mat, cv::Size(m_desireSize.width, m_desireSize.height));
	}

	if (m_enableThreshold)
	{
		cv::threshold(mat, mat, m_thresholdValue, 255, CV_THRESH_BINARY);
	}

	mat = mat.reshape(1, 1);
	mat.convertTo(mat, CV_32FC1);

	return mat;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTml::SetDesireSize(cv::Size size)
{
	m_desireSize = size;
}