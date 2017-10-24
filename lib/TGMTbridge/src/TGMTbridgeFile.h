#pragma once
#include <string>
using namespace System;
using namespace System::IO;

ref class TGMTbridgeFile
{
public:
	TGMTbridgeFile();

	static void OpenFolder(System::String^ folder);
	static System::String^ GetCurrentFolder();
	static void TGMTbridgeFile::WriteAllText(System::String^ fileName, System::String^ text);
	static void TGMTbridgeFile::WriteAllText(std::string fileName, std::string text);
	static void WriteResouceFileToDisk(System::String^ targetFile, System::String^ resourceFile);
};

