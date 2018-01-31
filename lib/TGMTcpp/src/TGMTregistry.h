#if defined(WIN32) || defined(WIN64) 
#pragma once
#include "stdafx.h"
#include "Windows.h"


#define GetTGMTregistry TGMTregistry::GetInstance

class TGMTregistry
{
	static TGMTregistry* m_instance;
	bool m_inited = false;


	HKEY hKey;

	
	int ReadValueInt(std::wstring key, int defaultValue);
	bool TGMTregistry::ReadValueBool(std::wstring key, bool defaultValue);
	std::wstring ReadValueWString(std::wstring key, const std::wstring defaultValue = L"");

	void PrintResult(LONG result);
public:
	TGMTregistry();
	~TGMTregistry();

	static TGMTregistry* GetInstance()
	{
		if (!m_instance)
			m_instance = new TGMTregistry();
		return m_instance;
	}

	void Init(std::string key);

	void WriteValueString(std::string key, std::string value);
	void WriteValueBool(std::string key, bool value);
	void WriteValueInt(std::string key, int value);
	void WriteValueFloat(std::string key, float value);

	int ReadValueInt(std::string key, int defaultValue = 0);
	bool ReadValueBool(std::string key,  bool defaultValue = false);
	std::string ReadValueString(std::string key, std::string defaultValue = "");
	float ReadValueFloat(std::string key, float defaultValue = 0);
};
#endif
