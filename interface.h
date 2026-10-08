#ifndef INTERFACE_H
#define INTERFACE_H

#include <map>
#include <string>
#include <vector>

#include "Array.h"
#include "linear_list.h"
#include "DoubleLinearList.h"
#include "stack.h"
#include "Queue.h"
#include "DoubleQueue.h"
#include "BST_tree.h"
#include "RB_tree.h"

struct ProgramData
{
	std::map<std::string, MArray*> arrays;
	std::map<std::string, FNode*> singleLists;
	std::map<std::string, DoublyLinkedList*> doubleLists;
	std::map<std::string, StackData*> stacks;
	std::map<std::string, QueueData*> queues;
	std::map<std::string, DoubleQueueData*> doubleQueues;
	std::map<std::string, BSTNode*> binaryTrees;
	std::map<std::string, RBNode*> trees;
};

bool RunCommand(ProgramData* data, const std::vector<std::string>& words,
	bool& changed, std::string& error);
void RunHelp();
bool loadProgramData(ProgramData* data, const std::string& fileName,
	std::string& error);
bool saveProgramData(ProgramData* data, const std::string& fileName,
	std::string& error);
void clearProgramData(ProgramData* data);

#endif
