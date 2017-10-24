#pragma once
#include <string>

namespace TGMT
{
	ref class TGMTbridgeUtil
	{
	public:
		
		static void ShowMessageBox(System::String^ title, System::String^ content);
		static void ShowMessageBox(std::string title, std::string content);
		static void ShowErrorBox(System::String^ title, System::String^ content);
		static void ShowErrorBox(std::string title, std::string content);


		static void OnlyInputNumber(Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e);
		static System::String^ RemoveSpecialCharacter(System::String^ str);
	};
	
}