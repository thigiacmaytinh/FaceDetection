#pragma once
#include <string>

ref class TGMTbridgeCryption
{
	static  cli::array<System::Byte>^ AES_Encrypt(cli::array<System::Byte>^ bytesToBeEncrypted, cli::array<System::Byte>^ passwordBytes);
	static  cli::array<System::Byte>^ AES_Decrypt(cli::array<System::Byte>^ bytesToBeDecrypted, cli::array<System::Byte>^ passwordBytes);
public:

	static System::String^ EncryptStringAES(System::String^ input, System::String^ password);
	static System::String^ DecryptStringAES(System::String^ input, System::String^ password);

	static std::string EncryptStringAES(std::string input, std::string password);
	static std::string DecryptStringAES(std::string input, std::string password);
};

