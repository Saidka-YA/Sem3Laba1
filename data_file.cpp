#include "interface.h"
#include "json_file.h"

#include <cstdlib>
#include <sstream>

using namespace std;

namespace
{

void writeString(stringstream& json, const string& value)
{
	json << '"';
	for (size_t i = 0; i < value.size(); ++i)
	{
		char c = value[i];
		if (c == '"' || c == '\\') json << '\\' << c;
		else if (c == '\n') json << "\\n";
		else if (c == '\r') json << "\\r";
		else if (c == '\t') json << "\\t";
		else json << c;
	}
	json << '"';
}

void writeRecord(stringstream& json, bool& first, const string& type,
	const string& name, const string& value, bool number = false)
{
	if (!first) json << ',';
	first = false;
	json << "{\"type\":";
	writeString(json, type);
	json << ",\"name\":";
	writeString(json, name);
	json << ",\"value\":";
	if (number) json << value;
	else writeString(json, value);
	json << '}';
}

void writeTree(stringstream& json, bool& first, const string& name, RBNode* node)
{
	if (node == nullptr) return;
	writeTree(json, first, name, node->left);
	writeRecord(json, first, "T", name, to_string(node->key), true);
	writeTree(json, first, name, node->right);
}

void writeStack(stringstream& json, bool& first, const string& name, StackNode* node)
{
	if (node == nullptr) return;
	writeStack(json, first, name, node->nextEl);
	writeRecord(json, first, "S", name, node->data);
}

void skipSpaces(const char*& cursor)
{
	while (*cursor == ' ' || *cursor == '\n' || *cursor == '\r' || *cursor == '\t') ++cursor;
}

bool expect(const char*& cursor, char symbol)
{
	skipSpaces(cursor);
	if (*cursor != symbol) return false;
	++cursor;
	return true;
}

bool readString(const char*& cursor, string& value)
{
	skipSpaces(cursor);
	if (*cursor != '"') return false;
	++cursor;
	value.clear();
	while (*cursor != '\0' && *cursor != '"')
	{
		if (*cursor != '\\') value += *cursor++;
		else
		{
			++cursor;
			if (*cursor == 'n') value += '\n';
			else if (*cursor == 'r') value += '\r';
			else if (*cursor == 't') value += '\t';
			else if (*cursor == '"' || *cursor == '\\' || *cursor == '/') value += *cursor;
			else return false;
			++cursor;
		}
	}
	if (*cursor != '"') return false;
	++cursor;
	return true;
}

bool readInteger(const char*& cursor, int& value)
{
	skipSpaces(cursor);
	char* end = nullptr;
	long number = strtol(cursor, &end, 10);
	if (end == cursor || number < -2147483647L - 1 || number > 2147483647L) return false;
	value = static_cast<int>(number);
	cursor = end;
	return true;
}

bool readRecord(const char*& cursor, string& type, string& name,
	string& value, int& key)
{
	string field;
	if (!expect(cursor, '{') || !readString(cursor, field) || field != "type" ||
		!expect(cursor, ':') || !readString(cursor, type) || !expect(cursor, ',') ||
		!readString(cursor, field) || field != "name" || !expect(cursor, ':') ||
		!readString(cursor, name) || !expect(cursor, ',') || !readString(cursor, field) ||
		field != "value" || !expect(cursor, ':')) return false;
	if (type == "T") return readInteger(cursor, key) && expect(cursor, '}');
	return readString(cursor, value) && expect(cursor, '}');
}

void deleteTree(RBNode* tree)
{
	if (tree == nullptr) return;
	deleteTree(tree->left);
	deleteTree(tree->right);
	delete tree;
}

} // namespace

bool saveProgramData(ProgramData* data, const string& fileName, string& error)
{
	stringstream json;
	bool first = true;
	json << '[';
	for (map<string, MArray*>::iterator it = data->arrays.begin(); it != data->arrays.end(); ++it)
		for (int i = 0; i < it->second->size; ++i)
			writeRecord(json, first, "M", it->first, it->second->data[i]);
	for (map<string, FNode*>::iterator it = data->singleLists.begin(); it != data->singleLists.end(); ++it)
		for (FNode* node = it->second; node != nullptr; node = node->nextEl)
			writeRecord(json, first, "F", it->first, node->data);
	for (map<string, DoublyLinkedList*>::iterator it = data->doubleLists.begin(); it != data->doubleLists.end(); ++it)
		for (LNode* node = it->second->head; node != nullptr; node = node->nextEl)
			writeRecord(json, first, "L", it->first, node->data);
	for (map<string, StackData*>::iterator it = data->stacks.begin(); it != data->stacks.end(); ++it)
		writeStack(json, first, it->first, it->second->head);
	for (map<string, DoubleQueueData*>::iterator it = data->queues.begin(); it != data->queues.end(); ++it)
		for (DoubleQueueNode* node = it->second->head; node != nullptr; node = node->nextEl)
			writeRecord(json, first, "Q", it->first, node->data);
	for (map<string, RBNode*>::iterator it = data->trees.begin(); it != data->trees.end(); ++it)
		writeTree(json, first, it->first, it->second);
	json << ']';
	return overwriteJsonFile(fileName, json.str(), error);
}

bool loadProgramData(ProgramData* data, const string& fileName, string& error)
{
	if (!ensureJsonFile(fileName, error)) return false;
	string json;
	if (!readJsonFile(fileName, json, error)) return false;
	const char* cursor = json.c_str();
	if (!expect(cursor, '[')) { error = "Ожидался JSON-массив."; return false; }
	skipSpaces(cursor);
	while (*cursor != ']')
	{
		string type, name, value;
		int key = 0;
		if (!readRecord(cursor, type, name, value, key))
		{
			error = "Не удалось прочитать элемент JSON.";
			return false;
		}
		if (type == "M")
		{
			if (data->arrays.find(name) == data->arrays.end())
			{
				data->arrays[name] = new MArray;
				MINIT(data->arrays[name]);
			}
			MPUSH(data->arrays[name], value);
		}
		else if (type == "F") FPUSHTAIL(data->singleLists[name], value);
		else if (type == "L")
		{
			if (data->doubleLists.find(name) == data->doubleLists.end())
			{
				data->doubleLists[name] = new DoublyLinkedList;
				LINIT(data->doubleLists[name]);
			}
			LPUSHTAIL(data->doubleLists[name], value);
		}
		else if (type == "S")
		{
			if (data->stacks.find(name) == data->stacks.end())
			{
				data->stacks[name] = new StackData;
				SINIT(data->stacks[name]);
			}
			SPUSH(data->stacks[name], value);
		}
		else if (type == "Q")
		{
			if (data->queues.find(name) == data->queues.end())
			{
				data->queues[name] = new DoubleQueueData;
				DQINIT(data->queues[name]);
			}
			DQPUSH(data->queues[name], value);
		}
		else if (type == "T") data->trees[name] = TINSERT(data->trees[name], key);
		else { error = "Неизвестный тип структуры в JSON."; return false; }
		skipSpaces(cursor);
		if (*cursor == ']') break;
		if (!expect(cursor, ',')) { error = "Ошибка между элементами JSON."; return false; }
	}
	if (!expect(cursor, ']')) { error = "Не закрыт JSON-массив."; return false; }
	skipSpaces(cursor);
	if (*cursor != '\0') { error = "Лишние данные после JSON-массива."; return false; }
	return true;
}

void clearProgramData(ProgramData* data)
{
	for (map<string, MArray*>::iterator it = data->arrays.begin(); it != data->arrays.end(); ++it)
	{
		MCLEAR(it->second);
		delete it->second;
	}
	for (map<string, FNode*>::iterator it = data->singleLists.begin(); it != data->singleLists.end(); ++it)
		while (it->second != nullptr) FDELHEAD(it->second);
	for (map<string, DoublyLinkedList*>::iterator it = data->doubleLists.begin(); it != data->doubleLists.end(); ++it)
	{
		while (it->second->head != nullptr) LDELHEAD(it->second);
		delete it->second;
	}
	for (map<string, StackData*>::iterator it = data->stacks.begin(); it != data->stacks.end(); ++it)
	{
		SCLEAR(it->second);
		delete it->second;
	}
	for (map<string, DoubleQueueData*>::iterator it = data->queues.begin(); it != data->queues.end(); ++it)
	{
		DQCLEAR(it->second);
		delete it->second;
	}
	for (map<string, RBNode*>::iterator it = data->trees.begin(); it != data->trees.end(); ++it)
		deleteTree(it->second);
	data->arrays.clear();
	data->singleLists.clear();
	data->doubleLists.clear();
	data->stacks.clear();
	data->queues.clear();
	data->trees.clear();
}
