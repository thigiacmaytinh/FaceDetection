#pragma once
#include "json/json.h"
#include <map>

#define GetTGMTConfig TGMTConfig::GetInstance
#define USE_JSON 0

class TGMTConfig
{
	static TGMTConfig* instance;
	

#if USE_JSON
	Json::Value m_root;
	
#else
	bool m_isLoadSuccess = false;
	std::map<std::string, std::string> m_stringList;
	std::string m_iniFile;

	std::string TGMTConfig::ReadSettingFromConfig(std::string iniFile, std::string section, std::string attribute);
#endif
public:
	static TGMTConfig* GetInstance()
	{
		if (!instance)
			instance = new TGMTConfig();
		return instance;
	}

	TGMTConfig();
	~TGMTConfig();

	bool LoadSettingFromFile(std::string settingFile);

#if USE_JSON

	bool LoadSettingFromString(std::string json);
	std::string ReadValueString(std::string key, std::string defaultValue = "");
	int ReadValueInt(std::string key, int defaultValue = 0);
	bool ReadValueBool(std::string key, bool defaultValue = false);
	float ReadValueFloat(std::string key, float defaultValue = 0.f);

#else
	
	std::string ReadValueString(std::string section, std::string key, std::string defaultValue = "");
	int ReadValueInt(std::string section, std::string key, int defaultValue = 0);
	bool ReadValueBool(std::string section, std::string key, bool defaultValue = false);
	float ReadValueFloat(std::string section, std::string key, float defaultValue = 0.f);

	void WriteConfigString(std::string section, std::string key, std::string value);
	void WriteConfigInt(std::string section, std::string key, int value);
	void WriteConfigFloat(std::string section, std::string key, float value);
	void WriteConfigBool(std::string section, std::string key, bool value);
#endif
	
	
};

