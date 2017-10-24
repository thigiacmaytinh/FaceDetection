#include "TGMTface.h"
#include "TGMTdebugger.h"
#include "TGMTConfig.h"
#include "TGMTutil.h"
#include "TGMTdebugger.h"
#include "TGMTfile.h"
#include "TGMTimage.h"
#include "TGMTutil.h"


TGMTface* TGMTface::m_instance = nullptr;
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#define NEW_FACE_RATIO 1.1


void EqualizeHist(cv::Mat& matInput)
{
	if (matInput.channels() == 1)
	{
		// Can bang histogram anh xam
		cv::equalizeHist(matInput, matInput);
	}
	else if (matInput.channels() == 3)
	{
		// Can bang histogram anh mau

		cv::Mat hsv, disp;
		cv::cvtColor(matInput, hsv, CV_BGR2HSV);
		std::vector<cv::Mat> hsv_channels;
		// Tach hsv thanh 3 kenh mau
		cv::split(hsv, hsv_channels);
		// Can bang histogram kenh mau v (value)
		cv::equalizeHist(hsv_channels[2], hsv_channels[2]);
		// Tron anh
		cv::merge(hsv_channels, hsv);
		// Chuyen doi hsv sang rgb de hien thi
		cv::cvtColor(hsv, disp, CV_HSV2BGR);

		matInput = disp;
	}
	else
	{
		ASSERT(false, "Only support image has 1 or 3 channels");
	}

}


void EnlargeFaceRectangle(std::vector<cv::Rect>& rects, cv::Size frameSize)
{
	for (int i = 0; i < rects.size(); i++)
	{
		int newW = rects[i].width * NEW_FACE_RATIO;
		int newH = rects[i].height * NEW_FACE_RATIO;


		cv::Rect rect;
		rect.x = rects[i].x - (newW - rects[i].width) / 2;
		rect.y = rects[i].y - (newH - rects[i].height) / 2;
		rect.width = newW;
		rect.height = newH;

		if (rect.x >= 0 && rect.y >= 0 && rect.x + rect.width <= frameSize.width && rect.y + rect.height <= frameSize.height)
		{
			rects[i] = rect;
		}
		
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

double GetFaceAngle(cv::Rect eyeLeft, cv::Rect eyeRight)
{
	cv::Point centerLeft = cv::Point (eyeLeft.x + eyeLeft.width / 2, eyeLeft.y + eyeLeft.height / 2);
	cv::Point centerRight = cv::Point(eyeRight.x + eyeRight.width / 2, eyeRight.y + eyeRight.height / 2);
	double eyeXdis = centerRight.x - centerLeft.x;
	double eyeYdis = centerRight.y - centerLeft.y;
	double angle = atan(eyeYdis / eyeXdis);
	double degree = angle * 180 / CV_PI;
	return degree;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTface::TGMTface()
{


}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTface::~TGMTface()
{
	mCascadeFace.~CascadeClassifier();
	mCascadeEyes.~CascadeClassifier();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTface::Init(std::string cascadeFace, std::string cascadeEye, FaceModel model)
{
	if (cascadeFace == "")
	{
		cascadeFace = GetTGMTConfig()->ReadValueString(INI_FACE_SECTION, "face_cascade_name");
	}
	
	ASSERT(!cascadeFace.empty(), "Face cascade name is null");	
	mCascadeFace = cv::CascadeClassifier(cascadeFace);
	ASSERT(!mCascadeFace.empty(), "Can not load face cascade: " + cascadeFace);

	m_shouldDetectEye = GetTGMTConfig()->ReadValueBool(INI_FACE_SECTION, "should_detect_eye");
	if (m_shouldDetectEye)
	{
		if (cascadeEye == "")
		{
			cascadeEye = GetTGMTConfig()->ReadValueString(INI_FACE_SECTION, "eye_cascade_name");
		}
		ASSERT(!cascadeEye.empty(), "Eye cascade name is null");
		mCascadeEyes = cv::CascadeClassifier(cascadeEye);
		ASSERT(!mCascadeEyes.empty(), "Can not load eye cascade: " + cascadeEye);
	}
	
	

	m_faceSize = GetTGMTConfig()->ReadValueInt(INI_FACE_SECTION, "face_size");	
	m_shouldDetectSkin = GetTGMTConfig()->ReadValueBool(INI_FACE_SECTION, "should_detect_skin");
	m_minFaceSize = GetTGMTConfig()->ReadValueInt(INI_FACE_SECTION, "min_face_size");
	m_maxFaceSize = GetTGMTConfig()->ReadValueInt(INI_FACE_SECTION, "max_face_size");
	m_correctFaceAngle = GetTGMTConfig()->ReadValueBool(INI_FACE_SECTION, "correct_face_angle");
	m_autoLuminance = GetTGMTConfig()->ReadValueBool(INI_FACE_SECTION, "auto_luminance");
	m_autoRetrain = GetTGMTConfig()->ReadValueBool(INI_FACE_SECTION, "auto_retrain"); 
	m_saveFolder = GetTGMTConfig()->ReadValueString(INI_FACE_SECTION, "save_folder");
	m_enableEqualizeHist = GetTGMTConfig()->ReadValueBool(INI_FACE_SECTION, "enable_equalizeHist");
	m_scaleFactor = GetTGMTConfig()->ReadValueFloat(INI_FACE_SECTION,"scale_factor", 1.1);
	m_minNeighbors = GetTGMTConfig()->ReadValueInt(INI_FACE_SECTION, "min_neighbors", 3);
	TGMTfile::CreateDir(m_saveFolder);

	if (model == undefine)
	{
		std::string modelName = GetTGMTConfig()->ReadValueString(INI_FACE_SECTION, "model");

		if (modelName == "eigen")
			model = Eigen;
		else if (modelName == "fisher")
			model = Fisher;
		else if (modelName == "LBPH")
			model = LBPH;
		else
			ASSERT(false, "Model name not defined");
	}
	
	if (!m_autoRetrain)
	{
		TGMTfile::CreateDir(m_saveFolder + "\\new");
	}

	SetModel(model);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<cv::Rect> TGMTface::DetectFaces(std::string fileInput)
{
	return DetectFaces(cv::imread(fileInput, CV_LOAD_IMAGE_GRAYSCALE));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<cv::Rect> TGMTface::DetectFaces(cv::Mat matInput)
{
	std::vector<cv::Rect> rects;
	if (matInput.data == nullptr)
	{
		PrintError("Input image is null");
		return rects;
	}
	//START_COUNT_TIME("detect_face");


	//scale to 1000 pixels
	float scaleRatio = 1.f;
	if (m_resizeInputImage)
	{
		if (matInput.cols > 1000)
		{
			scaleRatio = 1000.f / matInput.cols;
			cv::resize(matInput, matInput, cv::Size(1000, scaleRatio * matInput.rows));
		}
		else if (matInput.rows > 1000)
		{
			scaleRatio = 1000.f / matInput.rows;
			cv::resize(matInput, matInput, cv::Size(scaleRatio * matInput.cols, 1000));
		}
	}

	cv::Mat matGray = TGMTimage::ConvertToGray(matInput);
	if (m_enableEqualizeHist)
	{
		EqualizeHist(matGray);
	}
	
	mCascadeFace.detectMultiScale(matGray, rects, m_scaleFactor, m_minNeighbors, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(m_minFaceSize, m_minFaceSize), cv::Size(m_maxFaceSize, m_maxFaceSize) );
	if (rects.size() == 0)
		return rects;



	//detect eye to improve accuracy
	if (m_shouldDetectEye)
	{
		std::vector<cv::Rect> temp = rects;
		rects.clear();
		for (int i = 0; i < temp.size(); i++)
		{
			std::vector<cv::Rect> eyes = DetectEyes(matGray(temp[i]));
			if (eyes.size() >= 2)
			{
				rects.push_back(temp[i]);
#if _DEBUG
				cv::rectangle(matInput, cv::Rect(eyes[0].tl() + temp[0].tl(), eyes[0].br() + temp[0].tl()), cv::Scalar(0, 255, 0));
				cv::rectangle(matInput, cv::Rect(eyes[1].tl() + temp[0].tl(), eyes[1].br() + temp[0].tl()), cv::Scalar(0, 255, 0));
#endif
				if (m_correctFaceAngle)
				{
					TODO("implement face angle correction later");
					DEBUG_OUT("Angle %f", GetFaceAngle(eyes[0], eyes[1]));
				}
			}
		}
	}


	//return true size
	if (m_resizeInputImage)
	{
		if (scaleRatio != 1.f)
		{
			for (int i = 0; i < rects.size(); i++)
			{
				rects[i] = cv::Rect(rects[i].x / scaleRatio, rects[i].y / scaleRatio, rects[i].width / scaleRatio, rects[i].height / scaleRatio);
			}
		}
	}

	EnlargeFaceRectangle(rects, matInput.size());

	//DEBUG_IMAGE(matInput, "Mat input");

	//DEBUG_OUT("Detected %d faces", rects.size());
	//STOP_AND_PRINT_COUNT_TIME("detect_face");
	return rects;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<cv::Rect> TGMTface::DetectEyes(cv::Mat imgInput)
{
	ASSERT(imgInput.data, "TGMTobjDetect::DetectEyes: image input error");

	std::vector<cv::Rect> rects;
	mCascadeEyes.detectMultiScale(imgInput, rects, 1.2, 3, 0 | CV_HAAR_SCALE_IMAGE);

	return rects;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<TGMTface::Person> TGMTface::DetectPersons(cv::Mat matInput, std::string &errMsg, bool faceCropped)
{
	
	std::vector<cv::Rect> rects;

	return DetectPersons(matInput, rects, errMsg, faceCropped);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<TGMTface::Person> TGMTface::DetectPersons(cv::Mat matInput, std::vector<cv::Rect> &rects, std::string &errMsg, bool faceCropped)
{
	ASSERT(matInput.data != nullptr, "matinput is null");
	std::string name = "";
	std::vector<Person> result;

	if (m_autoLuminance)
	{
		cv::normalize(matInput, matInput,0,255, cv::NORM_MINMAX);
	}
	if (faceCropped)
	{
		rects.push_back(cv::Rect(0, 0, matInput.cols, matInput.rows));
	}
	else
	{
		rects = DetectFaces(matInput);
	}

	if (rects.size() == 0)
		errMsg = "Can not detect face";

	for (int i = 0; i < rects.size(); i++)
	{
		Person p = PredictPerson(matInput(rects[i]));
		p.rect = rects[i];
		result.push_back(p);
	}	

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTface::Person TGMTface::PredictPerson(cv::Mat faceInput)
{
	ASSERT(faceInput.data, "faceInput error");

	if (!m_trained || m_Persons.size() == 0)
	{
		return Person();
	}
	if ((m_model == Eigen || m_model == Fisher || m_model == LBPH) && m_pModel.empty())
	{
		return Person();
	}
	double confident;
		
	Person p = FindPerson(Predict(faceInput, confident));
	p.confident = confident;
	return p;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int TGMTface::Predict(cv::Mat face, double &confident)
{
	cv::Mat faceGray = TGMTimage::ConvertToGray(face);
	cv::resize(faceGray, faceGray, cv::Size(m_faceSize, m_faceSize));

	if (m_enableEqualizeHist)
	{
		EqualizeHist(faceGray);
	}
	int label;
	m_pModel->predict(faceGray, label, confident);

	return label;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTface::Training()
{
	return Training(m_saveFolder);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTface::Training(std::string dir)
{
	std::vector<std::string> subDirs = TGMTfile::GetChildDirInDir(dir);
	if (subDirs.size() == 0)
	{
#ifdef _MANAGED
		ShowMessageBox("Notice", "Training model fail: Folder is empty");
#else
		PrintError("Training model fail: Folder %s is empty", dir.c_str());
#endif
		return false;
	}
	else if (m_model == Fisher && subDirs.size() <= 2)
	{
#ifdef _MANAGED
		ShowMessageBox("Notice", "Fisher need least 2 persons");
#else
		PrintError("Fisher need least 2 persons");
#endif
		return false;
	}

	PrintMessage("Training data in directory %s", dir.c_str());

	START_COUNT_TIME("training face");
	m_Persons.clear();
	std::vector<cv::Mat> imgs;
	std::vector<int> labels;
	int count = 0;
	for (int i = 0; i < subDirs.size(); i++)
	{
		std::string currentDir = subDirs[i];

		if (currentDir == "new" || currentDir.substr(0, 5) == "Guest")
			continue;

		
		Person p;
		p.name = TGMTfile::GetFileName(currentDir);
		p.label = count++;
		p.isEmpty = false;

		PrintMessage("%d: %s", p.label + 1, currentDir.c_str());
		
		std::vector<std::string> imgList = TGMTfile::GetImageFilesInDir(dir + "\\" + currentDir);
		for (int j = 0; j < imgList.size(); j++)
		{
			cv::Mat img = cv::imread(imgList[j], CV_LOAD_IMAGE_GRAYSCALE);

			if (img.cols < m_faceSize || img.rows < m_faceSize)
			{
				PrintError("Can not train %s because size smaller than %d", imgList[j].c_str(), m_faceSize);
				continue;
			}				
			else if (img.cols > m_faceSize || img.rows > m_faceSize)
			{
				cv::resize(img, img, cv::Size(m_faceSize, m_faceSize));
			}				

			if (img.data)
			{
				imgs.push_back(img);
				labels.push_back(p.label);
				p.numImgs++;
			}
		}

		m_Persons.push_back(p);
	}
	if (imgs.size() < 2)
	{
		PrintError("Load images fail: not enough images to train, must bigger than 2");
		return false;
	}

	if (m_model == Eigen || m_model == Fisher || m_model == LBPH)
	{
		m_pModel->train(imgs, labels);
	}
	else
	{
		ASSERT(m_model != undefine, "Model must different undefine")
	}
	PrintSuccess("Train %d images success (%d ms)", imgs.size(), STOP_COUNT_TIME("training face"));


	m_trained = true;
	return m_trained;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTface::SaveModel()
{
	std::string modelName;
	if (m_model == Eigen)
	{
		modelName = "face_eigen.yaml";
	}
	else if (m_model == LBPH)
	{
		modelName = "face_LBPH.yaml";
	}
	else if (m_model == Fisher)
	{
		modelName = "face_fisher.yaml";
	}
	return SaveModel(modelName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTface::SaveModel(std::string modelFileName)
{
	m_pModel->save(modelFileName);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTface::LoadModel()
{
	std::string modelName;
	if (m_model == Eigen)
	{
		modelName = "face_eigen.yaml";
	}
	else if (m_model == LBPH)
	{
		modelName = "face_LBPH.yaml";
	}
	else if (m_model == Fisher)
	{
		modelName = "face_fisher.yaml";
	}
	return LoadModel(modelName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTface::LoadModel(std::string modelFileName)
{
	START_COUNT_TIME("face_load");
	if (!TGMTfile::FileExist(modelFileName))
	{
		PrintError("File %s does not exist", modelFileName.c_str());
		return false;
	}
	m_pModel->load(modelFileName);
	STOP_AND_PRINT_COUNT_TIME("face_load");
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTface::Person TGMTface::FindPerson(std::string name)
{
	for (int i = 0; i < m_Persons.size(); i++)
	{
		if (m_Persons[i].name == name)
			return m_Persons[i];
	}
	return Person();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTface::Person TGMTface::FindPerson(int label)
{
	for (int i = 0; i < m_Persons.size(); i++)
	{
		if (m_Persons[i].label == label)
			return m_Persons[i];
	}
	return Person();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTface::AddNewImage(cv::Mat matFace, std::string name, bool saveToIndividualDir)
{
	int label = FindPerson(name).label;
	if (label == -1)
	{
		label = m_Persons.size();		
	}	

	matFace = TGMTimage::ConvertToGray(matFace);
	cv::resize(matFace, matFace, cv::Size(m_faceSize, m_faceSize));
	if (m_enableEqualizeHist)
	{
		EqualizeHist(matFace);
	}

	if (m_autoRetrain)
	{
		if (saveToIndividualDir)
		{
			TGMTfile::CreateDir(m_saveFolder + "\\" + name);
			WriteImage(matFace, (m_saveFolder + "\\" + name + "\\" + GetCurrentDateTime(true) + ".jpg").c_str());
		}
		else
		{
			WriteImage(matFace, (m_saveFolder + "\\" + name + GetCurrentDateTime(true) + ".jpg").c_str());
		}

		std::vector<int> labels;
		labels.push_back(label);

		std::vector<cv::Mat> matFaces;
		matFaces.push_back(matFace);

		if (m_model == LBPH)
		{
			m_pModel->update(matFaces, labels);
		}
		else if (m_model == Eigen || m_model == Fisher)
		{
			Training(m_saveFolder);
		}

	}	
	else
	{
		WriteImage(matFace, (m_saveFolder + "\\new\\" + name + GetCurrentDateTime(true) + ".jpg").c_str());
	}

#ifndef _MANAGED
	PrintMessage("Added user %s", name.c_str());
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTface::SetModel(FaceModel model)
{
	if (m_model == model)
		return;

	if (model == Eigen)
	{
		m_pModel = createEigenFaceRecognizer();		
	}	
	else if (model == Fisher)
	{
		m_pModel = createFisherFaceRecognizer();		
	}
	else if (model == LBPH)
	{
		m_pModel = createLBPHFaceRecognizer();		
	}
	else
	{
		ASSERT(false, "Model must different undefine");
	}
	m_model = model;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTface::SetFaceSize(int faceSize)
{
	if (faceSize < m_minFaceSize || faceSize > m_minFaceSize)
		return;
	m_faceSize = faceSize;
}
