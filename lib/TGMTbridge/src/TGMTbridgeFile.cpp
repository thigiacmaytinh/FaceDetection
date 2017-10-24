#include "TGMTbridgeFile.h"
#include "TGMTbridge.h"

using namespace System::Diagnostics;
using namespace TGMT;

TGMTbridgeFile::TGMTbridgeFile()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTbridgeFile::OpenFolder(String^ folder)
{
	System::Diagnostics::Process::Start(folder);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String^ TGMTbridgeFile::GetCurrentFolder()
{
	return System::IO::Path::GetDirectoryName(System::Reflection::Assembly::GetEntryAssembly()->Location);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTbridgeFile::WriteAllText(System::String^ fileName, System::String^ text)
{
	System::Windows::Forms::SaveFileDialog^ ofd = gcnew System::Windows::Forms::SaveFileDialog();
	ofd->Filter = "Text file|*.txt";
	ofd->FileName = fileName;
	ofd->ShowHelp = true;
	ofd->Title = "Save text file";
	ofd->ShowDialog();
	System::String^ path = ofd->FileName;
	File::WriteAllText(path, text);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTbridgeFile::WriteAllText(std::string fileName, std::string text)
{
	System::String^ sfileName = TGMTbridge::stdStrToSystemStr(fileName);
	System::String^ stext = TGMTbridge::stdStrToSystemStr(text);

	WriteAllText(sfileName, stext);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTbridgeFile::WriteResouceFileToDisk(String^ targetFile, String^ resourceFile)
{
	Stream^ readStream = System::Reflection::Assembly::GetExecutingAssembly()->GetManifestResourceStream(resourceFile);
	if (readStream != nullptr)
	{
		FileStream^ writeStream = gcnew FileStream(targetFile, FileMode::Create);
		readStream->CopyTo(writeStream);
		readStream->Close();
		writeStream->Close(); // Required to flush the buffer & have non-zero filesize
	}
}