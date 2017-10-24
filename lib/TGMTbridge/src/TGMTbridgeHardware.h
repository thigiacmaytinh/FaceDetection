#pragma once
using namespace System;

namespace TGMT
{
	ref class TGMTbridgeHardware
	{
		static System::String^ GetCpuId();
		static System::String^ GetMainboardId();
		static System::String^ GetPartitionId(String^ partition);
		static System::String^ GetMacAddress();
	public:
		static System::String^ GetUDID();
		static System::String^ GetPCname();
		static System::String^ GetPCuserName();
	};

}