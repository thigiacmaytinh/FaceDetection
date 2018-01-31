#include "TGMThardware.h"
#if defined(WIN32) || defined(WIN64)
#include <tchar.h>
#include <Windows.h>
#endif
#include "TGMTutil.h"

#ifdef _MANAGED
#include "TGMTbridge.h"
#include "TGMTbridgeHardware.h"
#endif
#ifdef WIN32
#include <atlbase.h>
#include <wbemidl.h>
#include <comutil.h>
#include <Iptypes.h>
#include <wbemcli.h>
#include <comdef.h> 
#include <Iphlpapi.h>
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "iphlpapi.lib")
#endif
#define INFO_BUFFER_SIZE 32767

#ifdef OS_LINUX
typedef char TCHAR;
typedef unsigned long DWORD;
#endif

TGMThardware* TGMThardware::m_instance = nullptr;

//TGMThardware::TGMThardware()
//{
//}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//TGMThardware::~TGMThardware()
//{
//}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMThardware::GetUDID()
{
	if (m_udid == "")
	{
		m_udid = GetCpuId();
		m_udid += GetPartitionId("C:");

		std::string mainboardId = GetMainboardId();
		if (mainboardId.length() > 5)
			m_udid += mainboardId;

		m_udid += TGMTutil::RemoveSpecialCharacter(GetMacAddress());
		m_udid = TGMTutil::RemoveString(m_udid, ' ');

		int index = m_udid.length() / 2;
		m_udid = m_udid.substr(index) + m_udid.substr(0, index);
		m_udid = TGMTutil::RemoveSpecialCharacter(m_udid);
		m_udid = TGMTutil::ToLower(m_udid);
	}
	
	return m_udid;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMThardware::GetPCname()
{
#ifdef OS_LINUX
	return "Linux";
#else
	if (m_PCname == "")
	{
		TCHAR  infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		if (!GetComputerName(infoBuf, &bufCharCount))
			return "";
#ifdef UNICODE
		m_PCname = TGMTutil::WString2String(infoBuf);
#else
		m_PCname = infoBuf;
#endif
	}
	return m_PCname;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMThardware::GetCurrentUser()
{
#ifdef OS_LINUX
	return "ViscomLinux";
#else
	if (m_CurrentUser == "")
	{
		TCHAR  infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		if (!GetUserName(infoBuf, &bufCharCount))
			return "";
#ifdef UNICODE
		m_CurrentUser = TGMTutil::WString2String(infoBuf);
#else
		m_CurrentUser = infoBuf;
#endif
	}
	return m_CurrentUser;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMThardware::GetCpuId()
{
	if (m_CpuId == "")
		m_CpuId = QuerryWMI(L"ProcessorID", L"Win32_Processor");
	return m_CpuId;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMThardware::GetMainboardId()
{
	if (m_MainboardId == "")
		m_MainboardId = QuerryWMI(L"SerialNumber", L"Win32_BaseBoard");
	if (m_MainboardId == "To be filled by O.E.M.")
		m_MainboardId = "";
	return m_MainboardId;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMThardware::GetHDDId()
{
	if (m_HDDId == "")
		m_HDDId = QuerryWMI(L"SerialNumber", L"Win32_PhysicalMedia");
	return m_HDDId;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMThardware::GetMacAddress()
{
	#ifdef OS_LINUX
		return "00:00:00:00:00:00";
	#else
	if (m_MacAddress == "")
	{
		PIP_ADAPTER_INFO AdapterInfo;
		DWORD dwBufLen = sizeof(AdapterInfo);
		char *mac_addr = (char*)malloc(17);


		AdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
		if (AdapterInfo == NULL)
			mac_addr = "";


		// Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen     variable
		if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW)
		{
			AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);
			if (AdapterInfo == NULL)
				mac_addr = "";
		}

		if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR)
		{
			PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info
			do {
				sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
					pAdapterInfo->Address[0], pAdapterInfo->Address[1],
					pAdapterInfo->Address[2], pAdapterInfo->Address[3],
					pAdapterInfo->Address[4], pAdapterInfo->Address[5]);

				pAdapterInfo = pAdapterInfo->Next;
			} while (pAdapterInfo && mac_addr == "");
		}
		free(AdapterInfo);
		m_MacAddress = mac_addr;
	}
	return m_MacAddress;
	#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMThardware::QuerryWMI(std::wstring select, std::wstring from)
{
	#ifdef OS_LINUX
	return "LinuxUnknow";
	#else
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
		return "";

	// setup process-wide security context
	hr = CoInitializeSecurity(NULL, // we're not a server
		-1, // we're not a server
		NULL, // dito
		NULL, // reserved
		RPC_C_AUTHN_LEVEL_DEFAULT, // let DCOM decide
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE,
		NULL);

	std::string result = "";

	if (FAILED(hr))
		return result;


	// we're going to use CComPtr<>s, whose lifetime must end BEFORE CoUnitialize is called
	{
		// connect to WMI
		CComPtr< IWbemLocator > locator;
		hr = CoCreateInstance(CLSID_WbemAdministrativeLocator, NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWbemLocator, reinterpret_cast<void**>(&locator));
		if (SUCCEEDED(hr))
		{
			// connect to local service with current credentials
			CComPtr< IWbemServices > service;
			hr = locator->ConnectServer(L"root\\cimv2", NULL, NULL, NULL,
				WBEM_FLAG_CONNECT_USE_MAX_WAIT,
				NULL, NULL, &service);
			if (SUCCEEDED(hr))
			{
				// execute a query
				CComPtr< IEnumWbemClassObject > enumerator;
				hr = service->ExecQuery(L"WQL", (BSTR)(L"select " + select + L" from " + from).c_str(),
					WBEM_FLAG_FORWARD_ONLY, NULL, &enumerator);
				if (SUCCEEDED(hr))
				{
					// read the first instance from the enumeration (only one on single processor machines)
					CComPtr< IWbemClassObject > processor = NULL;
					ULONG retcnt;
					hr = enumerator->Next(WBEM_INFINITE, 1L, reinterpret_cast<IWbemClassObject**>(&processor), &retcnt);
					if (SUCCEEDED(hr))
					{
						if (retcnt > 0)
						{
							// we have a processor installed :)
							// now extract a property value
							_variant_t var_val;
							hr = processor->Get(select.c_str(), 0, &var_val, NULL, NULL);
							if (SUCCEEDED(hr))
							{
								if (var_val.vt == VT_NULL)
									result = "";
								else
									result = (const char*)_bstr_t(V_BSTR(&var_val));
							}
						}
					}
				}
			}
		}
	}
	CoUninitialize();

	return result;
	#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMThardware::GetPartitionId(char* partition)
{
	#ifdef OS_LINUX
	return "LinuxUnknow";
	#else
	if (m_PartitionID == "")
	{
		DWORD VolumeSerialNumber = 0;
#ifdef UNICODE
		GetVolumeInformation((LPCWSTR)partition, NULL, NULL, &VolumeSerialNumber, NULL, NULL, NULL, NULL);
#else
		GetVolumeInformation(partition, NULL, NULL, &VolumeSerialNumber, NULL, NULL, NULL, NULL);
#endif
		m_PartitionID = TGMTutil::FormatString("%X", VolumeSerialNumber);
	}
	return m_PartitionID;
	#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void TGMThardware::GetScreenResolution(int &width, int &height)
{
	#ifdef OS_LINUX
	width = 1024;
	height = 768;
	#else
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	width = desktop.right;
	height = desktop.bottom;
	#endif
}