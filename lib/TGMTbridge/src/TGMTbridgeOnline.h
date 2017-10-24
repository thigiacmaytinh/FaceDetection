#pragma once
#include <string>
using namespace System;

ref class TGMTbridgeOnline
{
public:
	//TGMTbridgeOnline();
	static System::String^ ReadOnlineFile(System::String^ url);
	static std::string ReadOnlineFile(std::string url);
	
	static bool CheckOnlineFileExist(System::String^ url);
	static bool CheckOnlineFileExist(std::string url);

	static System::String^ SendGETRequest(System::String^ url);
	static std::string SendGETRequest(std::string url);
};

