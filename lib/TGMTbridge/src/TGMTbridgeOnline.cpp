#include "TGMTbridgeOnline.h"
#include "TGMTbridge.h"

using namespace System::Net;
using namespace System::IO;
//TGMTbridgeOnline::TGMTbridgeOnline()
//{
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String^ TGMTbridgeOnline::ReadOnlineFile(String^ url)
{
	WebClient^ client = gcnew WebClient();
	Stream^ stream = client->OpenRead(url);
	StreamReader^ reader = gcnew StreamReader(stream);
	String^ content = reader->ReadToEnd();
	reader->Close();
	return content;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTbridgeOnline::ReadOnlineFile(std::string url)
{
	String^ sUrl = TGMT::TGMTbridge::stdStrToSystemStr(url);
	return TGMT::TGMTbridge::SystemStr2stdStr(ReadOnlineFile(sUrl));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTbridgeOnline::CheckOnlineFileExist(String^ url)
{
	bool result = true;
	HttpWebResponse^ response = nullptr;
	auto request = (HttpWebRequest^)WebRequest::Create(url);
	request->Method = "HEAD";


	try
	{
		response = (HttpWebResponse^)request->GetResponse();
	}
	catch (WebException^ ex)
	{
		result = false;
	}
	finally
	{
		// Don't forget to close your response.
		if (response != nullptr)
		{
			response->Close();
		}
	}
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTbridgeOnline::CheckOnlineFileExist(std::string url)
{
	String^ sUrl = TGMT::TGMTbridge::stdStrToSystemStr(url);
	return CheckOnlineFileExist(sUrl);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

System::String^ TGMTbridgeOnline::SendGETRequest(System::String^ url)
{
	auto request = (HttpWebRequest^)WebRequest::Create(url);
	HttpWebResponse^ response = nullptr;
	String^ content = "";
	try
	{
		response = (HttpWebResponse^)request->GetResponse();
		Stream^ stream = response->GetResponseStream();
		StreamReader^ reader = gcnew StreamReader(stream);
		content = reader->ReadToEnd();
	}
	catch (WebException^ ex)
	{
		
	}
	finally
	{
		// Don't forget to close your response.
		if (response != nullptr)
		{
			response->Close();
		}
	}
	return content;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTbridgeOnline::SendGETRequest(std::string url)
{
	String^ sUrl = TGMT::TGMTbridge::stdStrToSystemStr(url);
	return TGMT::TGMTbridge::SystemStr2stdStr(SendGETRequest(sUrl));
}