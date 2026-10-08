#include "interface.h"
#include "json_file.h"

#include <cstdlib>
#include <sstream>
#include <cstring>

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
	if (!first) json << ',';
	first = false;
	json << "{\"type\":\"T\",\"name\":";
	writeString(json, name);
	json << ",\"value\":" << node->key
		<< ",\"color\":" << node->color << ",\"parent\":";
	if (node->parent == nullptr) json << "null";
	else json << node->parent->key;
	json << '}';
	writeTree(json, first, name, node->left);
	writeTree(json, first, name, node->right);
}

void writeBinaryTree(stringstream& json, bool& first, const string& name, BSTNode* node)
{
	if (node == nullptr) return;
	writeBinaryTree(json, first, name, node->left);
	writeRecord(json, first, "B", name, to_string(node->data), true);
	writeBinaryTree(json, first, name, node->right);
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

bool readRecord(const char*& cursor, string& type, string& name, string& value,
	int& key, int& color, bool& hasParent, int& parentKey, bool& hasTreeShape)
{
	string field;
	hasParent = false;
	hasTreeShape = false;
	if (!expect(cursor, '{') || !readString(cursor, field) || field != "type" ||
		!expect(cursor, ':') || !readString(cursor, type) || !expect(cursor, ',') ||
		!readString(cursor, field) || field != "name" || !expect(cursor, ':') ||
		!readString(cursor, name) || !expect(cursor, ',') || !readString(cursor, field) ||
		field != "value" || !expect(cursor, ':')) return false;
	if (type != "T" && type != "B") return readString(cursor, value) && expect(cursor, '}');
	if (!readInteger(cursor, key)) return false;
	if (type == "B") return expect(cursor, '}');
	skipSpaces(cursor);
	if (*cursor == '}') { ++cursor; return true; }
	if (!expect(cursor, ',') || !readString(cursor, field) || field != "color" ||
		!expect(cursor, ':') || !readInteger(cursor, color) || !expect(cursor, ',') ||
		!readString(cursor, field) || field != "parent" || !expect(cursor, ':')) return false;
	skipSpaces(cursor);
	if (strncmp(cursor, "null", 4) == 0) cursor += 4;
	else { if (!readInteger(cursor, parentKey)) return false; hasParent = true; }
	if (!expect(cursor, '}')) return false;
	hasTreeShape = true;
	return true;
}

void deleteTree(RBNode* tree)
{
	if (tree == nullptr) return;
	deleteTree(tree->left);
	deleteTree(tree->right);
	delete tree;
}

void deleteBinaryTree(BSTNode* tree)
{
	if (tree == nullptr) return;
	deleteBinaryTree(tree->left);
	deleteBinaryTree(tree->right);
	delete tree;
}

} 

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
	for (map<string, QueueData*>::iterator it = data->queues.begin(); it != data->queues.end(); ++it)
		for (QueueNode* node = it->second->head; node != nullptr; node = node->nextEl)
			writeRecord(json, first, "Q", it->first, node->data);
	for (map<string, DoubleQueueData*>::iterator it = data->doubleQueues.begin(); it != data->doubleQueues.end(); ++it)
		for (DoubleQueueNode* node = it->second->head; node != nullptr; node = node->nextEl)
			writeRecord(json, first, "D", it->first, node->data);
	for (map<string, BSTNode*>::iterator it = data->binaryTrees.begin(); it != data->binaryTrees.end(); ++it)
		writeBinaryTree(json, first, it->first, it->second);
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
		int key = 0, color = BLACK, parentKey = 0;
		bool hasParent = false, hasTreeShape = false;
		if (!readRecord(cursor, type, name, value, key, color, hasParent, parentKey, hasTreeShape))
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
				data->queues[name] = new QueueData;
				QINIT(data->queues[name]);
			}
			QPUSH(data->queues[name], value);
		}
		else if (type == "D")
		{
			if (data->doubleQueues.find(name) == data->doubleQueues.end())
			{
				data->doubleQueues[name] = new DoubleQueueData;
				DQINIT(data->doubleQueues[name]);
			}
			DQPUSH(data->doubleQueues[name], value);
		}
		else if (type == "B") data->binaryTrees[name] = BSTINSERT(data->binaryTrees[name], key);
		else if (type == "T")
		{
			if (hasTreeShape) 
				data->trees[name] = TRESTORE(data->trees[name], key, static_cast<Color>(color), hasParent, parentKey);
			else data->trees[name] = TINSERT(data->trees[name], key);
		}
		else { error = "Неизвестный тип структуры в JSON."; return false; }
		skipSpaces(cursor);
		if (*cursor == ']') break;
		if (!expect(cursor, ',')) { 
			error = "Ошибка между элементами JSON."; 
			return false; 
		}
	}
	if (!expect(cursor, ']')) { 
		error = "Не закрыт JSON-массив."; 
		return false; 
	}
	skipSpaces(cursor);
	if (*cursor != '\0') { 
		error = "Лишние данные после JSON-массива."; 
		return false; 
	}
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
	for (map<string, QueueData*>::iterator it = data->queues.begin(); it != data->queues.end(); ++it)
	{
		QCLEAR(it->second);
		delete it->second;
	}
	for (map<string, DoubleQueueData*>::iterator it = data->doubleQueues.begin(); it != data->doubleQueues.end(); ++it)
	{
		DQCLEAR(it->second);
		delete it->second;
	}
	for (map<string, BSTNode*>::iterator it = data->binaryTrees.begin(); it != data->binaryTrees.end(); ++it)
		deleteBinaryTree(it->second);
	for (map<string, RBNode*>::iterator it = data->trees.begin(); it != data->trees.end(); ++it)
		deleteTree(it->second);
	data->arrays.clear();
	data->singleLists.clear();
	data->doubleLists.clear();
	data->stacks.clear();
	data->queues.clear();
	data->doubleQueues.clear();
	data->binaryTrees.clear();
	data->trees.clear();
}
