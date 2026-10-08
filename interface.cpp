#include "interface.h"

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

bool readInt(const string& text, int& value)
{
	value = atoi(text.c_str());
	return true;
}

MArray* getArray(ProgramData* data, const string& name)
{
	if (data->arrays.find(name) == data->arrays.end())
	{
		MArray* array = new MArray;
		MINIT(array);
		data->arrays[name] = array;
	}
	return data->arrays[name];
}

FNode*& getSingleList(ProgramData* data, const string& name)
{
	return data->singleLists[name];
}

DoublyLinkedList* getDoubleList(ProgramData* data, const string& name)
{
	if (data->doubleLists.find(name) == data->doubleLists.end())
	{
		DoublyLinkedList* list = new DoublyLinkedList;
		LINIT(list);
		data->doubleLists[name] = list;
	}
	return data->doubleLists[name];
}

StackData* getStack(ProgramData* data, const string& name)
{
	if (data->stacks.find(name) == data->stacks.end())
	{
		StackData* stack = new StackData;
		SINIT(stack);
		data->stacks[name] = stack;
	}
	return data->stacks[name];
}

DoubleQueueData* getQueue(ProgramData* data, const string& name)
{
	if (data->queues.find(name) == data->queues.end())
	{
		DoubleQueueData* queue = new DoubleQueueData;
		DQINIT(queue);
		data->queues[name] = queue;
	}
	return data->queues[name];
}

RBNode*& getTree(ProgramData* data, const string& name)
{
	return data->trees[name];
}

bool hasArgs(const vector<string>& words, size_t count, string& error)
{
	if (words.size() == count) return true;
	error = "Неверное количество аргументов.";
	return false;
}

bool getIndex(const vector<string>& words, int& index, string& error)
{
	if (words.size() < 3 || !readInt(words[2], index))
	{
		error = "Индекс должен быть целым числом.";
		return false;
	}
	return true;
}

bool RunMPush(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 3, error)) return false;
	MPUSH(getArray(data, words[1]), words[2]);
	return true;
}

bool RunMInsert(ProgramData* data, const vector<string>& words, string& error)
{
	int index;
	if (!hasArgs(words, 4, error) || !getIndex(words, index, error)) return false;
	MINSERT(getArray(data, words[1]), index, words[3]);
	return true;
}

bool RunMGet(ProgramData* data, const vector<string>& words, string& error)
{
	int index;
	if (!hasArgs(words, 3, error) || !getIndex(words, index, error)) return false;
	cout << "-> " << MGET(getArray(data, words[1]), index) << endl;
	return true;
}

bool RunMDel(ProgramData* data, const vector<string>& words, string& error)
{
	int index;
	if (!hasArgs(words, 3, error) || !getIndex(words, index, error)) return false;
	MDEL(getArray(data, words[1]), index);
	return true;
}

bool RunMSet(ProgramData* data, const vector<string>& words, string& error)
{
	int index;
	if (!hasArgs(words, 4, error) || !getIndex(words, index, error)) return false;
	MREPLACE(getArray(data, words[1]), index, words[3]);
	return true;
}

bool RunMLen(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 2, error)) return false;
	cout << "-> " << MLENGTH(getArray(data, words[1])) << endl;
	return true;
}

bool RunFPushHead(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 3, error)) return false;
	FPUSHHEAD(getSingleList(data, words[1]), words[2]);
	return true;
}

bool RunFPushTail(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 3, error)) return false;
	FPUSHTAIL(getSingleList(data, words[1]), words[2]);
	return true;
}

bool RunFPushAfter(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 4, error)) return false;
	FNode* node = FGET(getSingleList(data, words[1]), words[2]);
	if (node == nullptr) { error = "Опорный элемент не найден."; return false; }
	FINSERTAFTER(node, words[3]);
	return true;
}

bool RunFPushBefore(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 4, error)) return false;
	FNode*& head = getSingleList(data, words[1]);
	FINSERTBEFORE(head, FGET(head, words[2]), words[3]);
	return true;
}

bool RunFDelHead(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 2, error)) return false;
	FDELHEAD(getSingleList(data, words[1]));
	return true;
}

bool RunFDelTail(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 2, error)) return false;
	FDELTAIL(getSingleList(data, words[1]));
	return true;
}

bool RunFDelAfter(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 3, error)) return false;
	FNode* node = FGET(getSingleList(data, words[1]), words[2]);
	if (node == nullptr || node->nextEl == nullptr) { error = "Элемент для удаления не найден."; return false; }
	FDELAFTER(node);
	return true;
}

bool RunFDelBefore(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 3, error)) return false;
	FNode*& head = getSingleList(data, words[1]);
	FDELBEFORE(head, FGET(head, words[2]));
	return true;
}

bool RunFDelVal(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 3, error)) return false;
	FDELVALUE(getSingleList(data, words[1]), words[2]);
	return true;
}

bool RunFGet(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 3, error)) return false;
	FNode* node = FGET(getSingleList(data, words[1]), words[2]);
	if (node == nullptr) cout << "-> FALSE" << endl;
	else cout << "-> " << node->data << endl;
	return true;
}

bool RunLPushHead(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 3, error)) return false;
	LPUSHHEAD(getDoubleList(data, words[1]), words[2]);
	return true;
}

bool RunLPushTail(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 3, error)) return false;
	LPUSHTAIL(getDoubleList(data, words[1]), words[2]);
	return true;
}

bool RunLPushAfter(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 4, error)) return false;
	DoublyLinkedList* list = getDoubleList(data, words[1]);
	LINSERTAFTER(list, LGET(list, words[2]), words[3]);
	return true;
}

bool RunLPushBefore(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 4, error)) return false;
	DoublyLinkedList* list = getDoubleList(data, words[1]);
	LINSERTBEFORE(list, LGET(list, words[2]), words[3]);
	return true;
}

bool RunLDelHead(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 2, error)) return false;
	LDELHEAD(getDoubleList(data, words[1]));
	return true;
}

bool RunLDelTail(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 2, error)) return false;
	LDELTAIL(getDoubleList(data, words[1]));
	return true;
}

bool RunLDelAfter(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 3, error)) return false;
	DoublyLinkedList* list = getDoubleList(data, words[1]);
	LNode* node = LGET(list, words[2]);
	if (node == nullptr || node->nextEl == nullptr) { error = "Элемент для удаления не найден."; return false; }
	LDELNODE(list, node->nextEl);
	return true;
}

bool RunLDelBefore(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 3, error)) return false;
	DoublyLinkedList* list = getDoubleList(data, words[1]);
	LNode* node = LGET(list, words[2]);
	if (node == nullptr || node->prevEl == nullptr) { error = "Элемент для удаления не найден."; return false; }
	LDELNODE(list, node->prevEl);
	return true;
}

bool RunLDelVal(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 3, error)) return false;
	LDELVALUE(getDoubleList(data, words[1]), words[2]);
	return true;
}

bool RunLGet(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 3, error)) return false;
	LNode* node = LGET(getDoubleList(data, words[1]), words[2]);
	if (node == nullptr) cout << "-> FALSE" << endl;
	else cout << "-> " << node->data << endl;
	return true;
}

bool RunSPush(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 3, error)) return false;
	SPUSH(getStack(data, words[1]), words[2]);
	return true;
}

bool RunSPop(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 2, error)) return false;
	cout << "-> " << SPOP(getStack(data, words[1])) << endl;
	return true;
}

bool RunQPush(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 3, error)) return false;
	DQPUSH(getQueue(data, words[1]), words[2]);
	return true;
}

bool RunQPop(ProgramData* data, const vector<string>& words, string& error)
{
	if (!hasArgs(words, 2, error)) return false;
	cout << "-> " << DQPOP(getQueue(data, words[1])) << endl;
	return true;
}

bool RunTInsert(ProgramData* data, const vector<string>& words, string& error)
{
	if (words.size() != 3) { error = "Укажите имя дерева и ключ."; return false; }
	int key;
	if (!readInt(words[2], key)) { error = "Ключ должен быть целым числом."; return false; }
	RBNode*& tree = getTree(data, words[1]);
	tree = TINSERT(tree, key);
	return true;
}

bool RunTDel(ProgramData* data, const vector<string>& words, string& error)
{
	if (words.size() != 3) { error = "Укажите имя дерева и ключ."; return false; }
	int key;
	if (!readInt(words[2], key)) { error = "Ключ должен быть целым числом."; return false; }
	RBNode*& tree = getTree(data, words[1]);
	tree = TDEL(tree, key);
	return true;
}

bool RunTGet(ProgramData* data, const vector<string>& words, string& error)
{
	if (words.size() != 3) { error = "Укажите имя дерева и ключ."; return false; }
	int key;
	if (!readInt(words[2], key)) { error = "Ключ должен быть целым числом."; return false; }
	cout << "-> " << (TGET(getTree(data, words[1]), key) == nullptr ? "FALSE" : "TRUE") << endl;
	return true;
}

void printType(ProgramData* data, const string& type)
{
	if (type == "M" || type == "ALL") for (map<string, MArray*>::iterator it = data->arrays.begin(); it != data->arrays.end(); ++it)
	{
		cout << it->first << ": "; MPRINT(it->second);
	}
	if (type == "F" || type == "ALL") for (map<string, FNode*>::iterator it = data->singleLists.begin(); it != data->singleLists.end(); ++it)
	{
		cout << it->first << ":\n"; FPRINT(it->second); FPRINTREVERSE(it->second);
	}
	if (type == "L" || type == "ALL") for (map<string, DoublyLinkedList*>::iterator it = data->doubleLists.begin(); it != data->doubleLists.end(); ++it)
	{
		cout << it->first << ":\n"; LPRINT(it->second); LPRINTREVERSE(it->second);
	}
	if (type == "S" || type == "ALL") for (map<string, StackData*>::iterator it = data->stacks.begin(); it != data->stacks.end(); ++it)
	{
		cout << it->first << ":\n"; SPRINT(it->second);
	}
	if (type == "Q" || type == "ALL") for (map<string, DoubleQueueData*>::iterator it = data->queues.begin(); it != data->queues.end(); ++it)
	{
		cout << it->first << ":\n"; DQPRINT(it->second);
	}
	if (type == "T" || type == "ALL") for (map<string, RBNode*>::iterator it = data->trees.begin(); it != data->trees.end(); ++it)
	{
		cout << it->first << ":\n"; TPRINT(it->second);
	}
}

bool runPrint(const vector<string>& words, ProgramData* data, string& error)
{
	if (words.size() == 1) printType(data, "ALL");
	else if (words.size() == 2 && (words[1] == "ALL" || words[1] == "M" || words[1] == "F" ||
		words[1] == "L" || words[1] == "S" || words[1] == "Q" || words[1] == "T"))
		printType(data, words[1]);
	else if (words.size() == 2)
	{
		string name = words[1];
		if (data->arrays.count(name)) MPRINT(data->arrays[name]);
		else if (data->singleLists.count(name)) FPRINT(data->singleLists[name]);
		else if (data->doubleLists.count(name)) LPRINT(data->doubleLists[name]);
		else if (data->stacks.count(name)) SPRINT(data->stacks[name]);
		else if (data->queues.count(name)) DQPRINT(data->queues[name]);
		else if (data->trees.count(name)) TPRINT(data->trees[name]);
		else { error = "Структура не найдена."; return false; }
	}
	else { error = "Использование: PRINT <имя|тип|ALL>."; return false; }
	return true;
}



Command GetCommand(const std::string& name)
{
	if (name == "MPUSH") return Command::MPUSH;
	if (name == "MINSERT") return Command::MINSERT;
	if (name == "MGET") return Command::MGET;
	if (name == "MDEL") return Command::MDEL;
	if (name == "MSET" || name == "MREPLACE") return Command::MSET;
	if (name == "MLEN" || name == "MLENGTH") return Command::MLEN;
	if (name == "FPUSHHEAD") return Command::FPUSHHEAD;
	if (name == "FPUSHTAIL") return Command::FPUSHTAIL;
	if (name == "FPUSHAFTER") return Command::FPUSHAFTER;
	if (name == "FPUSHBEFORE") return Command::FPUSHBEFORE;
	if (name == "FDELHEAD") return Command::FDELHEAD;
	if (name == "FDELTAIL") return Command::FDELTAIL;
	if (name == "FDELAFTER") return Command::FDELAFTER;
	if (name == "FDELBEFORE") return Command::FDELBEFORE;
	if (name == "FDELVAL") return Command::FDELVAL;
	if (name == "FGET") return Command::FGET;
	if (name == "LPUSHHEAD") return Command::LPUSHHEAD;
	if (name == "LPUSHTAIL") return Command::LPUSHTAIL;
	if (name == "LPUSHAFTER") return Command::LPUSHAFTER;
	if (name == "LPUSHBEFORE") return Command::LPUSHBEFORE;
	if (name == "LDELHEAD") return Command::LDELHEAD;
	if (name == "LDELTAIL") return Command::LDELTAIL;
	if (name == "LDELAFTER") return Command::LDELAFTER;
	if (name == "LDELBEFORE") return Command::LDELBEFORE;
	if (name == "LDELVAL") return Command::LDELVAL;
	if (name == "LGET") return Command::LGET;
	if (name == "SPUSH") return Command::SPUSH;
	if (name == "SPOP") return Command::SPOP;
	if (name == "QPUSH") return Command::QPUSH;
	if (name == "QPOP") return Command::QPOP;
	if (name == "TINSERT") return Command::TINSERT;
	if (name == "TDEL") return Command::TDEL;
	if (name == "TGET") return Command::TGET;
	if (name == "ISMEMBER") return Command::TGET;
	if (name == "PRINT") return Command::PRINT;
	if (name == "HELP") return Command::HELP;
	if (name == "EXIT") return Command::EXIT;
	return Command::UNKNOWN;
}

bool RunPrint(ProgramData* data, const vector<string>& words, string& error)
{
	return runPrint(words, data, error);
}

void RunHelp()
{
	cout << "M: MPUSH MINSERT MGET MDEL MSET MLEN" << endl;
	cout << "F: FPUSHHEAD FPUSHTAIL FPUSHAFTER FPUSHBEFORE FDELHEAD FDELTAIL FDELAFTER FDELBEFORE FDELVAL FGET" << endl;
	cout << "L: LPUSHHEAD LPUSHTAIL LPUSHAFTER LPUSHBEFORE LDELHEAD LDELTAIL LDELAFTER LDELBEFORE LDELVAL LGET" << endl;
	cout << "S: SPUSH SPOP; Q: QPUSH QPOP; T: TINSERT TDEL TGET; PRINT; EXIT" << endl;
}
