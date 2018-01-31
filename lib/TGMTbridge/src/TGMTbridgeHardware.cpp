#include "TGMTbridgeHardware.h"
#using <system.management.dll>
#include "TGMTbridgeUtil.h"

using namespace System;
using namespace System::Management;
using namespace TGMT;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String^ TGMTbridgeHardware::GetCpuId()
{
	ManagementObjectSearcher^ mbs = gcnew ManagementObjectSearcher("Select ProcessorID From Win32_processor");
	ManagementObjectCollection^ mbsList = mbs->Get();
	String^ id = "";
	for each(ManagementObject^ mo in mbsList)
	{
		if (mo["ProcessorID"] != nullptr)
			id = mo["ProcessorID"]->ToString();
	}
	return id;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String^ TGMTbridgeHardware::GetMainboardId()
{
	ManagementObjectSearcher^ mos = gcnew ManagementObjectSearcher("SELECT SerialNumber FROM Win32_BaseBoard");
	ManagementObjectCollection^ moc = mos->Get();
	String^ serial = "";
	for each(ManagementObject^ mo in moc)
	{
		serial = mo["SerialNumber"]->ToString();
	}
	if (serial == "To be filled by O.E.M.")
		return "";
	return serial;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String^ TGMTbridgeHardware::GetPartitionId(String^ partition)
{
	ManagementObject^ dsk = gcnew ManagementObject("win32_logicaldisk.deviceid=\"" + partition + "\"");
	dsk->Get();
	String^ diskid = dsk["VolumeSerialNumber"]->ToString();
	return diskid;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String^ TGMTbridgeHardware::GetMacAddress()
{
	ManagementObjectSearcher^ objMOS = gcnew ManagementObjectSearcher("Select MacAddress FROM Win32_NetworkAdapterConfiguration");
	ManagementObjectCollection^ objMOC = objMOS->Get();

	for each(ManagementObject^ objMO in objMOC)
	{
		Object^ tempMacAddrObj = objMO["MacAddress"];

		if (tempMacAddrObj != nullptr) //Skip objects without a MACAddress
		{
			return tempMacAddrObj->ToString()->Replace(":", "");
		}
	}
	return "";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String^ TGMTbridgeHardware::GetUDID()
{
	String^ udid = GetCpuId();
	udid += GetPartitionId("C:");
	String^ mainboardId = GetMainboardId();
	
	if (mainboardId != "" && mainboardId->Length >5)
		udid += mainboardId;

	udid += GetMacAddress();
	udid->Replace(" ", "");

	int index = udid->Length / 2;
	udid = udid->Substring(index) + udid->Substring(0, index);
	udid = TGMTbridgeUtil::RemoveSpecialCharacter(udid);
	udid = udid->ToLower();
	return udid;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String^ TGMTbridgeHardware::GetPCname()
{
	return Environment::MachineName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String^ TGMTbridgeHardware::GetPCuserName()
{
	return Environment::UserName;
}