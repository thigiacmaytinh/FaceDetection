#include "TGMTConfig.h"
#include "TGMTdebugger.h"
#include <fstream>
#include <sstream>
#include <string>
#include "TGMTfile.h"
#include "TGMTutil.h"
#ifdef OS_LINUX
 #include <string.h>
#endif

TGMTConfig* TGMTConfig::instance = nullptr;

TGMTConfig::TGMTConfig()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////

TGMTConfig::~TGMTConfig()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTConfig::LoadSettingFromFile(std::string settingFile)
{
	if (settingFile.substr(1, 1) != ":")
	{
		settingFile = TGMTfile::GetCurrentDir() + "\\" + settingFile;
	}
	if (!TGMTfile::FileExist(settingFile))
	{
		WriteLog("TGMTconfig: missing settingFile %s", settingFile.c_str());
		ASSERT(false, "Missing config file: %s ", settingFile.c_str());
	}
	m_iniFile = settingFile;
	return m_reader.LoadSetting(settingFile);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

#if (defined(WIN32) || defined(WIN64)) && defined( UNICODE)

std::wstring TGMTConfig::ReadValueWString(std::string section, std::string key, std::wstring defaultValue)
{	
	std::wstring result = defaultValue;
	wchar_t retStr[1024] = { 0 };

	std::wstring winiFile = TGMTutil::String2WString(m_iniFile);
	std::wstring wsection = TGMTutil::String2WString(section);
	std::wstring wkey = TGMTutil::String2WString(key);
	DWORD sizeRead = GetPrivateProfileString(wsection.c_str(), wkey.c_str(), NULL,
		retStr, _countof(retStr), winiFile.c_str());

	if (sizeRead > 0)
		result = retStr;
	return result;
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////

std::string TGMTConfig::ReadValueString(std::string section, std::string key, std::string defaultValue)
{
	return m_reader.ReadValueString(section, key, defaultValue);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

int TGMTConfig::ReadValueInt(std::string section, std::string key, int defaultValue)
{
	return m_reader.ReadValueInt(section, key, defaultValue);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTConfig::ReadValueBool(std::string section, std::string key, bool defaultValue)
{
	return m_reader.ReadValueBool(section, key, defaultValue);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

double TGMTConfig::ReadValueDouble(std::string section, std::string key, double defaultValue)
{
	return m_reader.ReadValueDouble(section, key, defaultValue);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(WIN32) || defined(WIN64)

void TGMTConfig::WriteConfigString(std::string section, std::string key, std::string value)
{
#ifdef UNICODE
	std::wstring strValue = TGMTutil::String2WString(value);
#else
	std::string strValue = value;
#endif
	WritePrivateProfileString((TGMTSTR)section.c_str(), (TGMTSTR)key.c_str(), strValue.c_str(), (TGMTSTR)m_iniFile.c_str());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef UNICODE

void TGMTConfig::WriteConfigWString(std::string section, std::string key, std::wstring value)
{
	WritePrivateProfileString((TGMTSTR)section.c_str(), (TGMTSTR)key.c_str(), value.c_str(), (TGMTSTR)m_iniFile.c_str());
}

#endif

//////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTConfig::WriteConfigInt(std::string section, std::string key, int value)
{
#ifdef UNICODE
	std::wstringstream ss;
#else
	std::stringstream ss;
#endif
	ss << value;
	WritePrivateProfileString((TGMTSTR)section.c_str(), (TGMTSTR)key.c_str(), ss.str().c_str(), (TGMTSTR)m_iniFile.c_str());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTConfig::WriteConfigDouble(std::string section, std::string key, double value)
{
#ifdef UNICODE
	std::wstringstream ss;
#else
	std::stringstream ss;
#endif
	ss << value;
	WritePrivateProfileString((TGMTSTR)section.c_str(), (TGMTSTR)key.c_str(), ss.str().c_str(), (TGMTSTR)m_iniFile.c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TGMTConfig::WriteConfigBool(std::string section, std::string key, bool value)
{
#ifdef UNICODE
	WritePrivateProfileString((TGMTSTR)section.c_str(), (TGMTSTR)key.c_str(), value ? L"true" : L"false", (TGMTSTR)m_iniFile.c_str());
#else
	WritePrivateProfileString((TGMTSTR)section.c_str(), (TGMTSTR)key.c_str(), value ? "true" : "false", (TGMTSTR)m_iniFile.c_str());
#endif
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTConfig::LastReadSuccess()
{
	return m_reader.LastReadSuccess();
}
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////
// Read an INI file into easy-to-access name/value pairs.

// inih and INIReader are released under the New BSD license (see LICENSE.txt).
// Go to the project home page for more info:
//
// https://github.com/benhoyt/inih

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <stdio.h>

#if !INI_USE_STACK
#include <stdlib.h>
#endif

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

/* Nonzero if ini_handler callback should accept lineno parameter. */
#ifndef INI_HANDLER_LINENO
#define INI_HANDLER_LINENO 0
#endif

#define MAX_SECTION 50
#define MAX_NAME 50

/* Typedef for prototype of handler function. */
#if INI_HANDLER_LINENO
typedef int(*ini_handler)(void* user, char* section,
	char* name, char* value,
	int lineno);
#else
typedef int(*ini_handler)(void* user, char* section, char* name, char* value);
#endif

/* Typedef for prototype of fgets-style reader function. */
typedef char* (*ini_reader)(char* str, int num, void* stream);

/* Parse given INI-style file. May have [section]s, name=value pairs
(whitespace stripped), and comments starting with ';' (semicolon). Section
is "" if name=value pair parsed before any section heading. name:value
pairs are also supported as a concession to Python's configparser.

For each name=value pair parsed, call handler function with given user
pointer as well as section, name, and value (data only valid for duration
of handler call). Handler should return nonzero on success, zero on error.

Returns 0 on success, line number of first error on parse error (doesn't
stop on first error), -1 on file open error, or -2 on memory allocation
error (only when INI_USE_STACK is zero).
*/
int ini_parse(char* filename, ini_handler handler, void* user);

/* Same as ini_parse(), but takes a FILE* instead of filename. This doesn't
close the file when it's finished -- the caller must do that. */
int ini_parse_file(FILE* file, ini_handler handler, void* user);

/* Same as ini_parse(), but takes an ini_reader function pointer instead of
filename. Used for implementing custom or string-based I/O (see also
ini_parse_string). */
int ini_parse_stream(ini_reader reader, void* stream, ini_handler handler, void* user);

/* Same as ini_parse(), but takes a zero-terminated string with the INI data
instead of a file. Useful for parsing INI data from a network socket or
already in memory. */
int ini_parse_string(char* string, ini_handler handler, void* user);

/* Nonzero to allow multi-line value parsing, in the style of Python's
configparser. If allowed, ini_parse() will call the handler with the same
name for each subsequent line parsed. */
#ifndef INI_ALLOW_MULTILINE
#define INI_ALLOW_MULTILINE 1
#endif

/* Nonzero to allow a UTF-8 BOM sequence (0xEF 0xBB 0xBF) at the start of
the file. See http://code.google.com/p/inih/issues/detail?id=21 */
#ifndef INI_ALLOW_BOM
#define INI_ALLOW_BOM 1
#endif

/* Nonzero to allow inline comments (with valid inline comment characters
specified by INI_INLINE_COMMENT_PREFIXES). Set to 0 to turn off and match
Python 3.2+ configparser behaviour. */
#ifndef INI_ALLOW_INLINE_COMMENTS
#define INI_ALLOW_INLINE_COMMENTS 1
#endif
#ifndef INI_INLINE_COMMENT_PREFIXES
#define INI_INLINE_COMMENT_PREFIXES ";"
#endif

/* Nonzero to use stack, zero to use heap (malloc/free). */
#ifndef INI_USE_STACK
#define INI_USE_STACK 1
#endif

/* Stop parsing on first error (default is to keep parsing). */
#ifndef INI_STOP_ON_FIRST_ERROR
#define INI_STOP_ON_FIRST_ERROR 0
#endif

/* Maximum line length for any line in INI file. */
#ifndef INI_MAX_LINE
#define INI_MAX_LINE 200
#endif

TGMTConfig::INIReader::INIReader()
{

}

bool TGMTConfig::INIReader::LoadSetting(std::string settingFile)
{
	return 0 == ini_parse((char*)settingFile.c_str(), ValueHandler, this);
}

std::string TGMTConfig::INIReader::ReadValueString(std::string section, std::string key, std::string default_value)
{
	std::string keyname = MakeKey(section, key);
	// Use _values.find() here instead of _values.at() to support pre C++11 compilers

	std::string result = default_value;
	if (m_values.count(keyname) && !m_values.find(keyname)->second.empty())
	{
		result = m_values.find(keyname)->second;
		m_lastReadSuccess = true;
	}
	else
	{
		m_lastReadSuccess = false;

		if (m_writeDefaultValueIfNotExist)
		{
			GetTGMTConfig()->WriteConfigString(section, key, default_value);
		}
		
#ifdef DEBUG
		PrintMessageYellow("Missing config key: [%s] %s",section.c_str(),  key.c_str());
#endif
	}
	return result;
}

int TGMTConfig::INIReader::ReadValueInt(std::string section, std::string name, int default_value)
{
	std::string valstr = ReadValueString(section, name, TGMTutil::FormatString("%d", default_value));
	char* value = (char*)valstr.c_str();
	char* end;
	// This parses "1234" (decimal) and also "0x4D2" (hex)
	int n = strtol(value, &end, 0);
	return end > value ? n : default_value;
}

double TGMTConfig::INIReader::ReadValueDouble(std::string section, std::string name, double default_value)
{
	std::string valstr = ReadValueString(section, name, "");
	char* value = (char*)valstr.c_str();
	char* end;
	double n = strtod(value, &end);
	return end > value ? n : default_value;
}

bool TGMTConfig::INIReader::ReadValueBool(std::string section, std::string name, bool default_value)
{
	std::string valstr = ReadValueString(section, name, "");
	// Convert to lower case to make string comparisons case-insensitive
	std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
	if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
		return true;
	else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
		return false;
	else
		return default_value;
}

std::string TGMTConfig::INIReader::MakeKey(std::string section, std::string name)
{
	std::string key = section + "=" + name;
	// Convert to lower case to make section/name lookups case-insensitive
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);
	return key;
}

int TGMTConfig::INIReader::ValueHandler(void* user, char* section, char* name, char* value)
{
	INIReader* reader = (INIReader*)user;
	std::string key = MakeKey(section, name);
	if (reader->m_values[key].size() > 0)
		reader->m_values[key] += "\n";
	reader->m_values[key] += value;
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

bool TGMTConfig::INIReader::LastReadSuccess()
{
	return m_lastReadSuccess;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

/* Used by ini_parse_string() to keep track of string parsing state. */
typedef struct
{
	char* ptr;
	size_t num_left;
} ini_parse_string_ctx;

/* Strip whitespace chars off end of given string, in place. Return s. */
static char* rstrip(char* s)
{
	char* p = s + strlen(s);
	while (p > s && isspace((unsigned char)(*--p)))
		*p = '\0';
	return s;
}

/* Return pointer to first non-whitespace char in given string. */
static char* lskip(char* s)
{
	while (*s && isspace((unsigned char)(*s)))
		s++;
	return (char*)s;
}

/* Return pointer to first char (of chars) or inline comment in given string,
or pointer to null at end of string if neither found. Inline comment must
be prefixed by a whitespace character to register as a comment. */
static char* find_chars_or_comment(char* s, char* chars)
{
#if INI_ALLOW_INLINE_COMMENTS
	int was_space = 0;
	while (*s && (!chars || !strchr(chars, *s)) &&
		!(was_space && strchr(INI_INLINE_COMMENT_PREFIXES, *s))) {
		was_space = isspace((unsigned char)(*s));
		s++;
	}
#else
	while (*s && (!chars || !strchr(chars, *s))) {
		s++;
	}
#endif
	return (char*)s;
}

/* Version of strncpy that ensures dest (size bytes) is null-terminated. */
static char* strncpy0(char* dest, char* src, size_t size)
{
	strncpy(dest, src, size);
	dest[size - 1] = '\0';
	return dest;
}

/* See documentation in header file. */
int ini_parse_stream(ini_reader reader, void* stream, ini_handler handler, void* user)
{
	/* Uses a fair bit of stack (use heap instead if you need to) */
#if INI_USE_STACK
	char line[INI_MAX_LINE];
#else
	char* line;
#endif
	char section[MAX_SECTION] = "";
	char prev_name[MAX_NAME] = "";

	char* start;
	char* end;
	char* name;
	char* value;
	int lineno = 0;
	int error = 0;

#if !INI_USE_STACK
	line = (char*)malloc(INI_MAX_LINE);
	if (!line) {
		return -2;
	}
#endif

#if INI_HANDLER_LINENO
#define HANDLER(u, s, n, v) handler(u, s, n, v, lineno)
#else
#define HANDLER(u, s, n, v) handler(u, s, n, v)
#endif

	/* Scan through stream line by line */
	while (reader(line, INI_MAX_LINE, stream) != NULL)
	{
		lineno++;

		start = line;
#if INI_ALLOW_BOM
		if (lineno == 1 && (unsigned char)start[0] == 0xEF &&
			(unsigned char)start[1] == 0xBB &&
			(unsigned char)start[2] == 0xBF) {
			start += 3;
		}
#endif
		start = lskip(rstrip(start));

		if (*start == ';' || *start == '#')
		{
			/* Per Python configparser, allow both ; and # comments at the
			start of a line */
		}
#if INI_ALLOW_MULTILINE
		else if (*prev_name && *start && start > line) {
			/* Non-blank line with leading whitespace, treat as continuation
			of previous name's value (as per Python configparser). */
			if (!HANDLER(user, section, prev_name, start) && !error)
				error = lineno;
		}
#endif
		else if (*start == '[')
		{
			/* A "[section]" line */
			end = find_chars_or_comment(start + 1, "]");
			if (*end == ']') {
				*end = '\0';
				strncpy0(section, start + 1, sizeof(section));
				*prev_name = '\0';
			}
			else if (!error) {
				/* No ']' found on section line */
				error = lineno;
			}
		}
		else if (*start) {
			/* Not a comment, must be a name[=:]value pair */
			end = find_chars_or_comment(start, "=:");
			if (*end == '=' || *end == ':')
			{
				*end = '\0';
				name = rstrip(start);
				value = end + 1;
#if INI_ALLOW_INLINE_COMMENTS
				end = find_chars_or_comment(value, NULL);
				if (*end)
					*end = '\0';
#endif
				value = lskip(value);
				rstrip(value);

				/* Valid name[=:]value pair found, call handler */
				strncpy0(prev_name, name, sizeof(prev_name));
				if (!HANDLER(user, section, name, value) && !error)
					error = lineno;
			}
			else if (!error) {
				/* No '=' or ':' found on name[=:]value line */
				error = lineno;
			}
		}

#if INI_STOP_ON_FIRST_ERROR
		if (error)
			break;
#endif
	}

#if !INI_USE_STACK
	free(line);
#endif

	return error;
}

/* See documentation in header file. */
int ini_parse_file(FILE* file, ini_handler handler, void* user)
{
	return ini_parse_stream((ini_reader)fgets, file, handler, user);
}

/* See documentation in header file. */
int ini_parse(char* filename, ini_handler handler, void* user)
{
	FILE* file;
	int error;

	file = fopen(filename, "r");
	if (!file)
		return -1;
	error = ini_parse_file(file, handler, user);
	fclose(file);
	return error;
}

/* An ini_reader function to read the next line from a string buffer. This
is the fgets() equivalent used by ini_parse_string(). */
static char* ini_reader_string(char* str, int num, void* stream)
{
	ini_parse_string_ctx* ctx = (ini_parse_string_ctx*)stream;
	char* ctx_ptr = ctx->ptr;
	size_t ctx_num_left = ctx->num_left;
	char* strp = str;
	char c;

	if (ctx_num_left == 0 || num < 2)
		return NULL;

	while (num > 1 && ctx_num_left != 0)
	{
		c = *ctx_ptr++;
		ctx_num_left--;
		*strp++ = c;
		if (c == '\n')
			break;
		num--;
	}

	*strp = '\0';
	ctx->ptr = ctx_ptr;
	ctx->num_left = ctx_num_left;
	return str;
}

/* See documentation in header file. */
int ini_parse_string(char* string, ini_handler handler, void* user)
{
	ini_parse_string_ctx ctx;

	ctx.ptr = string;
	ctx.num_left = strlen(string);
	return ini_parse_stream((ini_reader)ini_reader_string, &ctx, handler, user);
}
