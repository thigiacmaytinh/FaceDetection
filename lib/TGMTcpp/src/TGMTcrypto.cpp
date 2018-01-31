#include "TGMTcrypto.h"
#include <openssl/aes.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <initializer_list>
#include "TGMTdebugger.h"
#include "TGMTutil.h"
#include "TGMTfile.h"

typedef unsigned char uchar;

#ifdef OS_LINUX
typedef unsigned char byte;
#else
typedef byte byte;
#endif
std::vector<byte> IV;

//TGMTcrypto::TGMTcrypto()
//{
//}
//
//
//TGMTcrypto::~TGMTcrypto()
//{
//}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string ConvertResult(std::string input)
{
	int i = 0;
	std::string result = "";
	while (input[i] != '\0')
	{
		result += TGMTutil::IntToString((int)input[i]) + " ";
		i++;
	}
	return result.substr(0, result.length() - 1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string ConvertInput(std::string input)
{
	std::string result = "";
	auto splits = TGMTutil::SplitString(input, ' ');
	for (size_t i = 0; i < splits.size(); i++)
	{
		int num = atoi(splits[i].c_str());
		char c = (char)num;
		result += c;
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

template <size_t multiple> size_t round_up(const size_t len)
{
	if (len % multiple == 0) return len;
	else return ((len / multiple) + 1) * multiple;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

// Make a Key of exactly 32 bytes, truncates or adds values if it's necessary
std::string AES_NormalizeKey(const void *const apBuffer, size_t aSize)
{
	static const byte key32[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
	const char *const Buffer = reinterpret_cast<const char *>(apBuffer);
	std::string Result(reinterpret_cast<const char *>(key32), 32);
	std::copy(Buffer, Buffer + ((aSize < 32) ? aSize : 32), Result.begin());
	return Result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

// Encrypt using AES cbc
std::string AESEncrypt(const void *const apBuffer, size_t aBufferSize, const void *const apKey, size_t aKeySize, std::vector<byte> &aIVector)
{
	// Create IVector.
	byte AES_IVector[AES_BLOCK_SIZE] = { 0 };


	// Create key.
	const std::string Key(AES_NormalizeKey(apKey, aKeySize));

	AES_KEY encryptKey;
	AES_set_encrypt_key(reinterpret_cast<const byte *>(Key.data()), 256, &encryptKey);

	// Encrypt.
	byte AES_Encrypted[1024999] = { 0 };
	AES_cbc_encrypt(static_cast<const byte *>(apBuffer), AES_Encrypted, aBufferSize, &encryptKey, AES_IVector, AES_ENCRYPT);
	const std::string encrypted(reinterpret_cast<const char *>(AES_Encrypted), round_up<AES_BLOCK_SIZE>(aBufferSize));
	// Finish.

	return encrypted;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

// Decrypt using AES cbc
std::string AESDecrypt(const void *const apBuffer, size_t aBufferSize, const void *const apKey, size_t aKeySize, std::vector<byte> &aIVector)
{
	// Read IVector.
	byte AES_IVector[AES_BLOCK_SIZE] = { 0 };
	std::copy(aIVector.begin(), aIVector.end(), std::begin(AES_IVector));

	// Create Key.
	const std::string key(AES_NormalizeKey(apKey, aKeySize));
	AES_KEY decryptKey;
	AES_set_decrypt_key(reinterpret_cast<const byte *>(key.data()), 256, &decryptKey);

	// Decrypt.
	byte AES_Decrypted[1024999] = { 0 };
	AES_cbc_encrypt(static_cast<const byte *>(apBuffer), AES_Decrypted, aBufferSize, &decryptKey, AES_IVector, AES_DECRYPT);
	const std::string decrypted(reinterpret_cast<const char *>(AES_Decrypted));
	return decrypted;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTcrypto::EncryptAES(std::string value, std::string password)
{
	std::string en = AESEncrypt(value.c_str(), value.length(), password.c_str(), password.length(), IV);
	return EncodeBase64(en);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTcrypto::DecryptAES(std::string value, std::string password)
{
	value = DecodeBase64(value);
	return AESDecrypt(value.c_str(), value.length(), password.c_str(), password.length(), IV);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

///////////////////////////////////////////////////////////////////////////////////////////////////

static inline bool is_base64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTcrypto::EncodeBase64(std::string value)
{
	byte *bytes_to_encode;// = new byte(value.length());

	//strcpy((char*)bytes_to_encode, value.c_str());
	bytes_to_encode = (byte*)value.c_str();


	int in_len = value.length();
	std::string ret;
	int i = 0;
	int j = 0;
	byte char_array_3[3];
	byte char_array_4[4];

	while (in_len--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i < 4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';

	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTcrypto::DecodeBase64(std::string encoded_string)
{
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;


	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
	{
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4)
		{
			for (i = 0; i < 4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
			{
				ret += char_array_3[i];
			}
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void printLastError()
{
	/*char * err = new char(130);;
	ERR_load_crypto_strings();
	ERR_error_string(ERR_get_error(), err);
	printf("RSA ERROR: %s\n", err);
	free(err);*/
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int padding = RSA_PKCS1_PADDING;

RSA * createRSA(std::string key, bool publickey)
{
	RSA *rsa = NULL;
	BIO *keybio;
	uchar* ukey = (uchar*)key.c_str();
	keybio = BIO_new_mem_buf(ukey, -1);
	if (keybio == NULL)
	{
		PrintError("Failed to create key BIO");
		return 0;
	}
	if (publickey)
	{
		rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	}
	else
	{
		rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	}
	if (rsa == NULL)
	{
		PrintError("Failed to create RSA");
	}

	return rsa;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int TGMTcrypto::public_encrypt(std::string data, std::string key, std::string& encrypted)
{
	RSA * rsa = createRSA(key, true);
	if (!rsa) return -1;
	uchar en[4098] = {};
	int result = RSA_public_encrypt(data.size(), (uchar*)data.c_str(), en, rsa, padding);
	encrypted = (char*)en;
	if (result == -1) printLastError();
	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int TGMTcrypto::private_decrypt(std::string enc_data, std::string key, std::string& decrypted)
{
	RSA * rsa = createRSA(key, false);
	if (!rsa) return -1;
	uchar de[4098] = {};
	int  result = RSA_private_decrypt(enc_data.size(), (uchar*)enc_data.c_str(), de, rsa, padding);
	decrypted = (char*)de;
	if (result == -1) printLastError();
	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int TGMTcrypto::private_encrypt(std::string data, std::string key, std::string& encrypted)
{
	RSA * rsa = createRSA(key, false);
	if (!rsa) return -1;
	uchar en[4098] = {};
	int result = RSA_private_encrypt(data.size(), (uchar*)data.c_str(), en, rsa, padding);
	encrypted = (char*)en;
	if (result == -1) printLastError();
	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int TGMTcrypto::public_decrypt(std::string enc_data, std::string key, std::string& decrypted)
{
	RSA * rsa = createRSA(key, true);
	if (!rsa) return -1;
	uchar de[4098] = {};
	int  result = RSA_public_decrypt(enc_data.size(), (uchar*)enc_data.c_str(), de, rsa, padding);
	decrypted = (char*)de;
	if (result == -1) printLastError();
	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTcrypto::LoadPublicKey(std::string file)
{
	std::string pubkey = TGMTutil::JoinVectorString(TGMTfile::ReadAllLine(file), "\n");
	ASSERT(!pubkey.empty(), "can not load RSA private key");
	return pubkey;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTcrypto::LoadPrivateKey(std::string file)
{
	std::string prikey = TGMTutil::JoinVectorString(TGMTfile::ReadAllLine(file), "\n");
	ASSERT(!prikey.empty(), "can not load RSA private key");
	return prikey;
}