#include "TGMTbridgeCryption.h"
#include "TGMTbridge.h"


using namespace System::Security::Cryptography;
using namespace System::Text;
using namespace System::IO;
using namespace System;

String^ TGMTbridgeCryption::EncryptStringAES(String^ input, String^ password)
{
	// Get the bytes of the String^
	auto bytesToBeEncrypted = Encoding::UTF8->GetBytes(input);
	auto passwordBytes = Encoding::UTF8->GetBytes(password);

	// Hash the password with SHA256
	passwordBytes = SHA256::Create()->ComputeHash(passwordBytes);

	array<System::Byte>^ bytesEncrypted = AES_Encrypt(bytesToBeEncrypted, passwordBytes);

	String^ result = Convert::ToBase64String(bytesEncrypted);

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

String^ TGMTbridgeCryption::DecryptStringAES(String^ input, String^ password)
{
	// Get the bytes of the String^
	array<System::Byte>^ bytesToBeDecrypted = Convert::FromBase64String(input);
	array<System::Byte>^ passwordBytes = Encoding::UTF8->GetBytes(password);
	passwordBytes = SHA256::Create()->ComputeHash(passwordBytes);

	auto bytesDecrypted = AES_Decrypt(bytesToBeDecrypted, passwordBytes);

	String^ result = Encoding::UTF8->GetString(bytesDecrypted);

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

array<Byte>^ TGMTbridgeCryption::AES_Encrypt(array<Byte>^ bytesToBeEncrypted, array<Byte>^ passwordBytes)
{
	array<Byte>^ encryptedBytes = nullptr;

	// Set your salt here, change it to meet your flavor:
	// The salt bytes must be at least 8 bytes.
	array<Byte>^ saltBytes = { 1, 2, 3, 4, 5, 6, 7, 8 };

	MemoryStream^ ms = gcnew MemoryStream();

	RijndaelManaged^ AES = gcnew RijndaelManaged();

	AES->KeySize = 256;
	AES->BlockSize = 128;

	Rfc2898DeriveBytes^ key = gcnew Rfc2898DeriveBytes(passwordBytes, saltBytes, 1000);
	AES->Key = key->GetBytes(AES->KeySize / 8);
	AES->IV = key->GetBytes(AES->BlockSize / 8);

	AES->Mode = CipherMode::CBC;

	CryptoStream^ cs = gcnew CryptoStream(ms, AES->CreateEncryptor(), CryptoStreamMode::Write);

	cs->Write(bytesToBeEncrypted, 0, bytesToBeEncrypted->Length);
	cs->Close();

	encryptedBytes = ms->ToArray();
	return encryptedBytes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

array<Byte>^ TGMTbridgeCryption::AES_Decrypt(array<Byte>^ bytesToBeDecrypted, array<Byte>^ passwordBytes)
{
	array<Byte>^ decryptedBytes = nullptr;

	// Set your salt here, change it to meet your flavor:
	// The salt bytes must be at least 8 bytes.
	array<Byte>^ saltBytes = { 1, 2, 3, 4, 5, 6, 7, 8 };

	MemoryStream^ ms = gcnew MemoryStream();

	RijndaelManaged^ AES = gcnew RijndaelManaged();

	AES->KeySize = 256;
	AES->BlockSize = 128;

	Rfc2898DeriveBytes^ key = gcnew Rfc2898DeriveBytes(passwordBytes, saltBytes, 1000);
	AES->Key = key->GetBytes(AES->KeySize / 8);
	AES->IV = key->GetBytes(AES->BlockSize / 8);

	AES->Mode = CipherMode::CBC;

	auto cs = gcnew CryptoStream(ms, AES->CreateDecryptor(), CryptoStreamMode::Write);

	cs->Write(bytesToBeDecrypted, 0, bytesToBeDecrypted->Length);
	cs->Close();

	decryptedBytes = ms->ToArray();

	return decryptedBytes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTbridgeCryption::EncryptStringAES(std::string input, std::string password)
{
	System::String^ SstrInput = TGMT::TGMTbridge::stdStrToSystemStr(input);
	System::String^ SstrPassword = TGMT::TGMTbridge::stdStrToSystemStr(password);
	System::String^ SstrOutput = EncryptStringAES(SstrInput, SstrPassword);
	return TGMT::TGMTbridge::SystemStr2stdStr(SstrOutput);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTbridgeCryption::DecryptStringAES(std::string input, std::string password)
{
	System::String^ SstrInput = TGMT::TGMTbridge::stdStrToSystemStr(input);
	System::String^ SstrPassword = TGMT::TGMTbridge::stdStrToSystemStr(password);
	System::String^ SstrOutput = DecryptStringAES(SstrInput, SstrPassword);
	return TGMT::TGMTbridge::SystemStr2stdStr(SstrOutput);
}