#include "json_file.h"

#include <fstream>
#include <sstream>

bool readJsonFile(const std::string& fileName, std::string& json, std::string& error)
{
	std::ifstream file(fileName.c_str());
	if (!file)
	{
		error = "Не удалось открыть файл: " + fileName;
		return false;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	json = buffer.str();
	return true;
}

bool ensureJsonFile(const std::string& fileName, std::string& error)
{
	std::ifstream input(fileName.c_str());
	if (input) return true;
	return overwriteJsonFile(fileName, "[]", error);
}

bool overwriteJsonFile(const std::string& fileName, const std::string& json,
	std::string& error)
{
	std::ofstream file(fileName.c_str(), std::ios::out | std::ios::trunc);
	if (!file)
	{
		error = "Не удалось записать файл: " + fileName;
		return false;
	}
	file << json << '\n';
	if (!file)
	{
		error = "Ошибка записи файла: " + fileName;
		return false;
	}
	return true;
}

bool appendJsonFile(const std::string& fileName, const std::string& jsonValue,
	std::string& error)
{
	std::string json;
	if (!readJsonFile(fileName, json, error))
	{
		return overwriteJsonFile(fileName, "[" + jsonValue + "]", error);
	}
	std::size_t start = json.find_first_not_of(" \t\r\n");
	std::size_t end = json.find_last_not_of(" \t\r\n");
	if (start == std::string::npos)
	{
		return overwriteJsonFile(fileName, "[" + jsonValue + "]", error);
	}
	if (json[start] != '[' || json[end] != ']')
	{
		error = "Для добавления файл должен содержать JSON-массив.";
		return false;
	}
	std::string contents = json.substr(0, end);
	std::size_t item = json.find_first_not_of("[ \t\r\n", start + 1);
	if (item != std::string::npos && item < end) contents += ',';
	contents += jsonValue + json.substr(end);
	return overwriteJsonFile(fileName, contents, error);
}
