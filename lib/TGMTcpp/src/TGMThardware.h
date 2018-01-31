#pragma once
#include <string>

#define GetTGMTHardware TGMThardware::GetInstance

class TGMThardware
{
	std::string QuerryWMI(std::wstring select, std::wstring from);
	static TGMThardware* m_instance;

	std::string m_CpuId;
	std::string m_MainboardId;
	std::string m_HDDId;
	std::string m_MacAddress;
	std::string m_UDID;
	std::string m_PCname;
	std::string m_CurrentUser;
	std::string m_PartitionID;
	std::string m_udid;
public:
	//TGMThardware();
	//~TGMThardware();

	std::string GetCpuId();
	std::string GetMainboardId();
	std::string GetHDDId();
	std::string GetMacAddress();
	std::string GetUDID();
	std::string GetPCname();
	std::string GetCurrentUser();

	//example: C:, D:
	std::string GetPartitionId(char* partition);

	static void GetScreenResolution(int &width, int &height);

	static TGMThardware* GetInstance()
	{
		if (!m_instance)
			m_instance = new TGMThardware();
		return m_instance;
	}
};

