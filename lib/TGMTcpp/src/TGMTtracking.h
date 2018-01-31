#pragma once
#include <string>
#ifdef _MANAGED
#include <pplawait.h>
#endif

#define GetTGMTtracking TGMTtracking::GetInstance

class TGMTtracking
{
	std::string m_appName;;
	std::string m_appVersion;
	std::string m_user;
	std::string m_pass;
	static TGMTtracking* m_instance;

	bool ReadConfig(std::string jsonContent);

public:

	static TGMTtracking* GetInstance()
	{
		if (!m_instance)
			m_instance = new TGMTtracking();
		return m_instance;
	}
	TGMTtracking();
	~TGMTtracking();

	void SetUserPass(std::string user, std::string pass);
	bool UploadFile(std::string filePath, std::string childDir, std::string customFileName, std::string targetURL);

#ifdef _MANAGED
	Concurrency::task<void> UploadFileAsync(std::string filePath, std::string childDir, std::string customFileName, std::string targetURL);
#else
	void UploadFileAsync(std::string filePath, std::string childDir, std::string customFileName, std::string targetURL);
#endif


#ifdef _MANAGED
	ref class TGMTtrackingManaged
	{

		void UploadFile(std::string filePath, std::string childDir, std::string customFileName, std::string targetURL)
		{
			GetTGMTtracking()->UploadFile(filePath, childDir, customFileName, targetURL);

		}
	};
#endif
};

