#include "TGMTutil.h"
#include <sstream>
#include <string>
#ifdef WIN32
#include "windows.h"
#endif
#include "TGMTdebugger.h"
#include <algorithm>
#include <random>
#ifdef OS_LINUX
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#endif
#define DEBUG_OUT_BUFFER_SIZE			(64*1024)

//TGMTutil::TGMTutil()
//{
//}
//
//
//TGMTutil::~TGMTutil()
//{
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTutil::IntToString(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::wstring TGMTutil::String2WString(const std::string s)
{
#if 0
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
#else
	return std::wstring(s.begin(), s.end());
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTutil::WString2String(const std::wstring s)
{
	return std::string(s.begin(), s.end());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTutil::FormatString(const char* fmt, ...)
{
	va_list arg_list;
	char str[DEBUG_OUT_BUFFER_SIZE];
	va_start(arg_list, fmt);
	vsnprintf(str, DEBUG_OUT_BUFFER_SIZE - 1, fmt, arg_list);
	return str;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ANDROID
std::string TGMTutil::GetParameter(int argc, char* argv[], char* key, char* defaultValue)
{
	if (argv[0] == nullptr)
		return defaultValue;
	
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], key) == 0)
		{
			if (argv[i + 1] != nullptr)
				return argv[i + 1];
			else
				return defaultValue;
		}
	}
	return defaultValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool TGMTutil::CheckParameterExist(int argc, char* argv[], char* key)
{
	if (TGMTutil::GetParameter(argc, argv, key) == "")
	{
		PrintError("Missing value of parameter %s", key);
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTutil::CheckParameterAloneExist(int argc, char* argv[], char* key)
{
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], key) == 0)
		{		
			return true;
		}
	}
	return false;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTutil::ReplaceString(std::string &inputStr, char oldchar, char newchar)
{
	std::replace(inputStr.begin(), inputStr.end(), oldchar, newchar);
	return inputStr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> TGMTutil::SplitString(std::string str, char separator)
{
	std::vector<std::string> elems;
	std::stringstream ss;
	ss.str(str);
	std::string item;
	while (std::getline(ss, item, separator)) {
		elems.push_back(item);
	}
	return elems;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int TGMTutil::GetRandomInt(int min, int max)
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<> dist(min, max);
	return dist(rng);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTutil::JoinVectorString(std::vector<std::string> strings, char* separator)
{
	
	if (strings.size() == 0)
		return "";

	std::string result = "";

	for (size_t i = 0; i < strings.size(); i++)
	{
		result += strings[i];
		if (i < strings.size() - 1)
		{
			result += separator;
		}
	}
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTutil::RemoveString(std::string inputStr, char chrWantRemove)
{
	inputStr.erase(std::remove(inputStr.begin(), inputStr.end(), chrWantRemove), inputStr.end());
	return inputStr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTutil::RemoveSpecialCharacter(std::string inputStr)
{
	inputStr.resize(std::remove_if(inputStr.begin(), inputStr.end(), [](char x) {return !isalnum(x) && !isspace(x); }) - inputStr.begin());
	return inputStr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTutil::ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}
