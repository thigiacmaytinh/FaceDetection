#include "TGMTtracking.h"
#include <curl/curl.h>
#include "TGMTonline.h"
#include "TGMTConfig.h"
#include "TGMTdebugger.h"
#include "TGMTfile.h"

#include <thread>
#ifdef _MANAGED
#else
#include <future>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#endif
TGMTtracking* TGMTtracking::m_instance = nullptr;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTtracking::TGMTtracking()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGMTtracking::~TGMTtracking()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTtracking::SetUserPass(std::string user, std::string pass)
{
	m_user = user;
	m_pass = pass;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTtracking::UploadFile(std::string filePath, std::string _childDir, std::string customFileName, std::string targetURL)
{
	ASSERT(m_user != "" & m_pass != "", "username and password are empty");
	ASSERT(targetURL != "" , "host url is empty");
	if (!TGMTfile::FileExist(filePath))
		return false;

	CURL *curl;
	CURLcode res;
	bool result = false;

	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist = NULL;
	static const char buf[] = "Expect:";

	curl_global_init(CURL_GLOBAL_ALL);

	/* Fill in the file upload field */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "sendfile",
		CURLFORM_FILE, filePath.c_str(),
		CURLFORM_END);

	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "childfolder",
		CURLFORM_COPYCONTENTS, _childDir.c_str(),
		CURLFORM_END);

	/* Fill in the custom filename field */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "customname",
		CURLFORM_COPYCONTENTS, customFileName.c_str(),
		CURLFORM_END);



	curl = curl_easy_init();
	/* initialize custom header list (stating that Expect: 100-continue is not
	wanted */
	headerlist = curl_slist_append(headerlist, buf);
	if (curl) 
	{
		/* what URL that receives this POST */
		curl_easy_setopt(curl, CURLOPT_URL, targetURL.c_str());
		//curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/upfile/dosavefile.php");

		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
		/* set user name and password for the authentication */
		curl_easy_setopt(curl, CURLOPT_USERPWD, (m_user + ":" + m_pass).c_str());
		//MinhDPT rem 
		//if ((argc == 2) && (!strcmp(argv[1], "noexpectheader")))
		//	/* only disable 100-continue header if explicitly requested */
		//	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		//END MinhDPT rem

		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			result = false;
		}
		else
		{
			result = true;
		}

		/* always cleanup */
		curl_easy_cleanup(curl);

		/* then cleanup the formpost chain */
		curl_formfree(formpost);
		/* free slist */
		curl_slist_free_all(headerlist);
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef _MANAGED
Concurrency::task<void>
#else
void 
#endif
TGMTtracking::UploadFileAsync(std::string filePath, std::string childDir, std::string customFileName, std::string targetURL)
{
#ifdef _MANAGED
	
	return Concurrency::create_task([filePath, childDir, customFileName, targetURL]()
	{
		GetTGMTtracking()->UploadFile(filePath, childDir, customFileName, targetURL);
	});
	
#else
	std::async([&](std::string _filePath, std::string _childDir, std::string _customFileName, std::string _targetURL)
	{
		return UploadFile(_filePath, _childDir, _customFileName, _targetURL);
	}, filePath, childDir, customFileName, targetURL);
#endif
}