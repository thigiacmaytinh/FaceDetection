#pragma once
#include <string>
class TGMTonline
{
public:
	//TGMTonline();
	//~TGMTonline();
	
	static bool IsOnlineFileExist(std::string url);
	static std::string ReadOnlineFile(std::string url);
	static bool SendGETRequest(std::string url, std::string user = "", std::string pass = "");
	static void SendGETRequestAsync(std::string url, std::string user = "", std::string pass = "");
	static bool IsInternetAvailable(std::string urlToCheck = "http://google.com");
};

