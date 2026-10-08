#include "interface.h"

#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char* argv[])
{
	string fileName = "data.json";
	vector<string> queries;
	for (int i=1; i<argc; ++i)
	{
		string option=argv[i];
		if (option=="--file" && i+1<argc) fileName=argv[++i];
		else if (option=="--query" && i+1<argc) queries.push_back(argv[++i]);
		else { 
			cerr << "Использование: ./dbms --file имя.json --query 'КОМАНДА аргументы'" << endl; 
			return 1; 
		}
	}
	if (queries.empty()) { cerr << "Укажите команду через --query." << endl; return 1; }

	ProgramData data = {};
	string error;
	if (!loadProgramData(&data,fileName,error)) { 
		cerr << "Ошибка: " << error << endl; 
		return 1; 
	}
	bool allSuccess=true;
	for (size_t i=0; i<queries.size(); ++i)
	{
		stringstream input(queries[i]);
		vector<string> words;
		string word;
		while (input>>word) words.push_back(word);
		bool changed=false;
		if (!RunCommand(&data,words,changed,error)) { 
			cerr << "Ошибка: " << error << endl; 
			allSuccess=false; 
		}
		else if (changed && !saveProgramData(&data,fileName,error)) { 
			cerr << "Ошибка: " << error << endl; 
			allSuccess=false; 
		}
		if (!words.empty() && words[0]=="EXIT") break;
	}
	clearProgramData(&data);
	return allSuccess ? 0 : 1;
}
