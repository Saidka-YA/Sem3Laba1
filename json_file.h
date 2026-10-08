#ifndef JSON_FILE_H
#define JSON_FILE_H

#include <string>

bool readJsonFile(const std::string& fileName, std::string& json, std::string& error);
bool ensureJsonFile(const std::string& fileName, std::string& error);
bool overwriteJsonFile(const std::string& fileName, const std::string& json,
	std::string& error);
bool appendJsonFile(const std::string& fileName, const std::string& jsonValue,
	std::string& error);

#endif
