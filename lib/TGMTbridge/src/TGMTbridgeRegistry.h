#pragma once
#include <string>

using namespace System;
using namespace Microsoft::Win32;

#define GetTGMTbridgeRegistry TGMTbridgeRegistry::GetInstance
ref class TGMTbridgeRegistry
{
	static TGMTbridgeRegistry^ mInstance = nullptr;


	TGMTbridgeRegistry();
	RegistryKey^ mRegKey = nullptr;
public:

	static TGMTbridgeRegistry^ GetInstance()
	{
		if (mInstance == nullptr)
			mInstance = gcnew TGMTbridgeRegistry();
		return mInstance;
	}

	void Init(System::String^ regKey);
	void Init(std::string regKey);

	void WriteValue(System::String^ value, bool valueData);
	void WriteValue(std::string value, bool valueData);
	void WriteValue(System::String^ value, System::String^ valueData);
	void WriteValue(std::string value, std::string valueData);
	void WriteValue(System::String^ value, int valueData);
	void WriteValue(std::string value, int valueData);


	//return false if value not exist
	String^ ReadValueString(System::String^ key, System::String^ defaultValue);
	std::string ReadValueString(std::string key, std::string defaultValue);
	int ReadValueInt(System::String^ key, int defaultValue);
	int ReadValueInt(std::string key, int defaultValue);
	bool ReadValueBool(System::String^ key, bool defaultValue);
	bool ReadValueBool(std::string key, bool defaultValue);
};

