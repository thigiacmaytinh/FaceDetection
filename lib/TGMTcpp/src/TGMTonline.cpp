#include "TGMTonline.h"
#include <curl/curl.h>
#include <future>
#include "TGMTdebugger.h"
#include <string.h>
//TGMTonline::TGMTonline()
//{
//}
//
//
//TGMTonline::~TGMTonline()
//{
//}

struct MemoryStruct {
	char *memory;
	size_t size;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

size_t write_memory_callback(char *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL)
	{
#ifdef SHOW_TIZEN_IAP_LOG
		dlog_print(DLOG_DEBUG, "_GET_FEED_", "not enough memory (realloc returned NULL)");
#endif //SHOW_TIZEN_IAP_LOG
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTonline::ReadOnlineFile(std::string url)
{
	CURL *curl_handle;
	CURLcode res;

	struct MemoryStruct chunk;
	chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
	chunk.size = 0;    /* no data at this point */

	curl_global_init(CURL_GLOBAL_ALL);
	curl_handle = curl_easy_init();
	if (curl_handle)
	{
		curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_memory_callback);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
		res = curl_easy_perform(curl_handle);
		if (res != CURLE_OK)
		{
#ifdef SHOW_TIZEN_IAP_LOG
			dlog_print(DLOG_DEBUG, "_GET_FEED_", "URL: %s", url);
			dlog_print(DLOG_DEBUG, "_GET_FEED_", "curl_easy_perform() failed: %s", curl_easy_strerror((CURLcode)res));
#endif //SHOW_TIZEN_IAP_LOG
		}
	}
	std::string resultString = (chunk.memory);
	
	//UnlockProfiles::getInstance()->set_feed_length(chunk.size);
	//UnlockProfiles::getInstance()->set_feed_data(strdup(chunk.memory));
	curl_easy_cleanup(curl_handle);
	free(chunk.memory);
	curl_global_cleanup();
	return resultString;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTonline::IsOnlineFileExist(std::string url)
{
	CURL *curl;
	CURLcode res;

	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();
	if (curl) {
		/* First set the URL that is about to receive our POST. This URL can
		just as well be a https:// URL if that is what should receive the
		data. */
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();
	return res != CURLE_REMOTE_FILE_NOT_FOUND;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTonline::SendGETRequestAsync(std::string url, std::string user, std::string pass)
{
	std::async(SendGETRequest, url, user, pass);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTonline::SendGETRequest(std::string url, std::string user, std::string pass)
{
	CURL* curl;
	curl = curl_easy_init();
	if (!curl)
	{
		PrintError("Can not init");
		return false;
	}
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_DNS_SERVERS, "8.8.8.8,8.8.4.4");
	if (user != "" && pass != "")
	{
		curl_easy_setopt(curl, CURLOPT_USERPWD, (user + ":" + pass).c_str());
	}		

	CURLcode res = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	return res == CURLE_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTonline::IsInternetAvailable(std::string urlToCheck)
{
	CURL *curl;
	CURLcode res;

	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();
	if (curl) {
		/* First set the URL that is about to receive our POST. This URL can
		just as well be a https:// URL if that is what should receive the
		data. */
		curl_easy_setopt(curl, CURLOPT_URL, urlToCheck.c_str());
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();
	return res == CURLE_OK;
}