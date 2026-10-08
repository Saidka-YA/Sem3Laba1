#ifndef INTERFACE_H
#define INTERFACE_H

#include <map>
#include <string>
#include <vector>

#include "Array.h"
#include "linear_list.h"
#include "DoubleLinearList.h"
#include "stack.h"
#include "DoubleQueue.h"
#include "RB_tree.h"

enum class Command
{
	MPUSH, MINSERT, MGET, MDEL, MSET, MLEN,
	FPUSHHEAD, FPUSHTAIL, FPUSHAFTER, FPUSHBEFORE,
	FDELHEAD, FDELTAIL, FDELAFTER, FDELBEFORE, FDELVAL, FGET,
	LPUSHHEAD, LPUSHTAIL, LPUSHAFTER, LPUSHBEFORE,
	LDELHEAD, LDELTAIL, LDELAFTER, LDELBEFORE, LDELVAL, LGET,
	SPUSH, SPOP,
	QPUSH, QPOP,
	TINSERT, TDEL, TGET,
	PRINT, HELP, EXIT, UNKNOWN
};

struct ProgramData
{
	std::map<std::string, MArray*> arrays;
	std::map<std::string, FNode*> singleLists;
	std::map<std::string, DoublyLinkedList*> doubleLists;
	std::map<std::string, StackData*> stacks;
	std::map<std::string, DoubleQueueData*> queues;
	std::map<std::string, RBNode*> trees;
};

Command GetCommand(const std::string& commandName);
bool RunMPush(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunMInsert(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunMGet(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunMDel(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunMSet(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunMLen(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunFPushHead(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunFPushTail(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunFPushAfter(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunFPushBefore(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunFDelHead(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunFDelTail(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunFDelAfter(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunFDelBefore(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunFDelVal(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunFGet(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunLPushHead(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunLPushTail(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunLPushAfter(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunLPushBefore(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunLDelHead(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunLDelTail(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunLDelAfter(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunLDelBefore(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunLDelVal(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunLGet(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunSPush(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunSPop(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunQPush(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunQPop(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunTInsert(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunTDel(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunTGet(ProgramData*, const std::vector<std::string>&, std::string&);
bool RunPrint(ProgramData*, const std::vector<std::string>&, std::string&);
void RunHelp();
bool loadProgramData(ProgramData* data, const std::string& fileName,
	std::string& error);
bool saveProgramData(ProgramData* data, const std::string& fileName,
	std::string& error);
void clearProgramData(ProgramData* data);

#endif
