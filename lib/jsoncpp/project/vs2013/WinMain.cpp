#include "json\json.h"
#include <fstream>

void main()
{
	Json::Value root;
	Json::Reader reader;

	std::ifstream is("config.json", std::ifstream::binary);
	bool success = reader.parse(is, root);
}