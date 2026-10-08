#include "interface.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	string fileName = "data.json";
	vector<string> queries;
	for (int i = 1; i < argc; ++i)
	{
		string argument = argv[i];
		if (argument == "--file" && i + 1 < argc) fileName = argv[++i];
		else if (argument == "--query" && i + 1 < argc) queries.push_back(argv[++i]);
		else
		{
			cerr << "Использование: ./dbms --file имя.json --query 'КОМАНДА аргументы'" << endl;
			return 1;
		}
	}

	ProgramData data = {};
	string error;
	if (!loadProgramData(&data, fileName, error))
	{
		cerr << "Ошибка: " << error << endl;
		clearProgramData(&data);
		return 1;
	}

	bool interactive = queries.empty();
	bool allSuccess = true;
	while (interactive || !queries.empty())
	{
		string query;
		if (interactive)
		{
			cout << "> ";
			if (!getline(cin, query)) break;
		}
		else
		{
			query = queries.front();
			queries.erase(queries.begin());
		}

		stringstream queryStream(query);
		vector<std::string> words;
		string word;
		while (queryStream >> word) words.push_back(word);
		if (words.empty()) continue;

		Command command = GetCommand(words[0]);
		bool changed = false;
		bool success = false;
		error.clear();
		switch (command)
		{
		case Command::MPUSH: success = RunMPush(&data, words, error); changed = true; break;
		case Command::MINSERT: success = RunMInsert(&data, words, error); changed = true; break;
		case Command::MGET: success = RunMGet(&data, words, error); break;
		case Command::MDEL: success = RunMDel(&data, words, error); changed = true; break;
		case Command::MSET: success = RunMSet(&data, words, error); changed = true; break;
		case Command::MLEN: success = RunMLen(&data, words, error); break;
		case Command::FPUSHHEAD: success = RunFPushHead(&data, words, error); changed = true; break;
		case Command::FPUSHTAIL: success = RunFPushTail(&data, words, error); changed = true; break;
		case Command::FPUSHAFTER: success = RunFPushAfter(&data, words, error); changed = true; break;
		case Command::FPUSHBEFORE: success = RunFPushBefore(&data, words, error); changed = true; break;
		case Command::FDELHEAD: success = RunFDelHead(&data, words, error); changed = true; break;
		case Command::FDELTAIL: success = RunFDelTail(&data, words, error); changed = true; break;
		case Command::FDELAFTER: success = RunFDelAfter(&data, words, error); changed = true; break;
		case Command::FDELBEFORE: success = RunFDelBefore(&data, words, error); changed = true; break;
		case Command::FDELVAL: success = RunFDelVal(&data, words, error); changed = true; break;
		case Command::FGET: success = RunFGet(&data, words, error); break;
		case Command::LPUSHHEAD: success = RunLPushHead(&data, words, error); changed = true; break;
		case Command::LPUSHTAIL: success = RunLPushTail(&data, words, error); changed = true; break;
		case Command::LPUSHAFTER: success = RunLPushAfter(&data, words, error); changed = true; break;
		case Command::LPUSHBEFORE: success = RunLPushBefore(&data, words, error); changed = true; break;
		case Command::LDELHEAD: success = RunLDelHead(&data, words, error); changed = true; break;
		case Command::LDELTAIL: success = RunLDelTail(&data, words, error); changed = true; break;
		case Command::LDELAFTER: success = RunLDelAfter(&data, words, error); changed = true; break;
		case Command::LDELBEFORE: success = RunLDelBefore(&data, words, error); changed = true; break;
		case Command::LDELVAL: success = RunLDelVal(&data, words, error); changed = true; break;
		case Command::LGET: success = RunLGet(&data, words, error); break;
		case Command::SPUSH: success = RunSPush(&data, words, error); changed = true; break;
		case Command::SPOP: success = RunSPop(&data, words, error); changed = true; break;
		case Command::QPUSH: success = RunQPush(&data, words, error); changed = true; break;
		case Command::QPOP: success = RunQPop(&data, words, error); changed = true; break;
		case Command::TINSERT: success = RunTInsert(&data, words, error); changed = true; break;
		case Command::TDEL: success = RunTDel(&data, words, error); changed = true; break;
		case Command::TGET: success = RunTGet(&data, words, error); break;
		case Command::PRINT: success = RunPrint(&data, words, error); break;
		case Command::HELP:
			RunHelp();
			success = true;
			break;
		case Command::EXIT:
			success = true;
			break;
		default:
			error = "Неизвестная команда: " + words[0];
			break;
		}

		if (!success)
		{
			allSuccess = false;
			if (!error.empty()) cerr << "Ошибка: " << error << endl;
		}
		if (success && changed && !saveProgramData(&data, fileName, error))
		{
			cerr << "Ошибка: " << error << endl;
			allSuccess = false;
		}
		if (command == Command::EXIT) break;
	}

	clearProgramData(&data);
	return allSuccess ? 0 : 1;
}
