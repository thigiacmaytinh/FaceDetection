#include "TGMTConfig.h"
#include "TGMTdebugger.h"
#include <fstream>
#include <sstream>
#include <string>
#include "TGMTfile.h"

TGMTConfig* TGMTConfig::instance = nullptr;

TGMTConfig::TGMTConfig()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////

TGMTConfig::~TGMTConfig()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTConfig::LoadSettingFromFile(std::string settingFile)
{
	ASSERT(TGMTfile::FileExist(settingFile), "Missing config file: %s ", settingFile.c_str());

#if USE_JSON
	Json::Reader reader;

	std::ifstream is(settingFile, std::ifstream::binary);
	m_isLoadSuccess = reader.parse(is, m_root);
	return m_isLoadSuccess;
#else
	m_iniFile = TGMTfile::GetCurrentDir() + "\\" + settingFile;
	return true;
#endif	
}

//////////////////////////////////////////////////////////////////////////////////////////////////

#if USE_JSON

bool TGMTConfig::LoadSettingFromString(std::string json)
{
	Json::Reader reader;
	m_isLoadSuccess = reader.parse(json, m_root);
	return m_isLoadSuccess;
}


//////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTConfig::ReadValueString(std::string key, std::string defaultValue)
{	
	if (!m_isLoadSuccess || !m_root[key])
		return defaultValue;
	return m_root[key].asString();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

int TGMTConfig::ReadValueInt(std::string key, int defaultValue)
{
	if (!m_isLoadSuccess || !m_root[key])
		return defaultValue;
	return m_root[key].asInt();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTConfig::ReadValueBool(std::string key, bool defaultValue)
{
	if (!m_isLoadSuccess || !m_root[key])
		return defaultValue;
	return m_root[key].asBool();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

float TGMTConfig::ReadValueFloat(std::string key, float defaultValue)
{
	if (!m_isLoadSuccess || !m_root[key])
		return defaultValue;
	return m_root[key].asFloat();	
}

//////////////////////////////////////////////////////////////////////////////////////////////////

#else

std::string TGMTConfig::ReadSettingFromConfig(std::string iniFile, std::string section, std::string attribute)
{
	char retStr[1024] = { 0 };
	std::string toRet = "";
	DWORD sizeRead = GetPrivateProfileString(section.c_str(), attribute.c_str(), NULL,
		retStr, _countof(retStr), iniFile.c_str());
	if (sizeRead > 0)
		toRet = retStr;

	return toRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTConfig::ReadValueString(std::string section, std::string key, std::string defaultValue)
{
	std::string temp = ReadSettingFromConfig(m_iniFile, section, key);
	if (temp != "")
		return temp;
	else
		return defaultValue;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

int TGMTConfig::ReadValueInt(std::string section, std::string key, int defaultValue)
{
	std::string temp = ReadSettingFromConfig(m_iniFile, section, key);
	if (temp != "")
		return atoi(temp.c_str());
	else
		return defaultValue;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTConfig::ReadValueBool(std::string section, std::string key, bool defaultValue)
{
	std::string temp = ReadSettingFromConfig(m_iniFile, section, key);
	if (temp == "true")
		return true;
	else if (temp == "false")
		return false;
	else
		return defaultValue;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

float TGMTConfig::ReadValueFloat(std::string section, std::string key, float defaultValue)
{
	std::string temp = ReadSettingFromConfig(m_iniFile, section, key);
	if (temp != "")	
		return atof(temp.c_str());
	else
		return defaultValue;	
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTConfig::WriteConfigString(std::string section, std::string key, std::string value)
{
	WritePrivateProfileString(section.c_str(), key.c_str(), value.c_str(), m_iniFile.c_str());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTConfig::WriteConfigInt(std::string section, std::string key, int value)
{
	std::stringstream ss;
	ss << value;
	WritePrivateProfileString(section.c_str(), key.c_str(), ss.str().c_str(), m_iniFile.c_str());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTConfig::WriteConfigFloat(std::string section, std::string key, float value)
{
	std::stringstream ss;
	ss << value;
	WritePrivateProfileString(section.c_str(), key.c_str(), ss.str().c_str(), m_iniFile.c_str());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTConfig::WriteConfigBool(std::string section, std::string key, bool value)
{
	WritePrivateProfileString(section.c_str(), key.c_str(), value ? "true" : "false", m_iniFile.c_str());
}
#endif