#include "interface.h"

#include <cstdlib>
#include <iostream>

using namespace std;

static MArray* getArray(ProgramData* d, const string& n)
{
	if (!d->arrays.count(n)) { d->arrays[n] = new MArray; MINIT(d->arrays[n]); }
	return d->arrays[n];
}

static FNode*& singleList(ProgramData* d, const string& n) { return d->singleLists[n]; }

static DoublyLinkedList* doubleList(ProgramData* d, const string& n)
{
	if (!d->doubleLists.count(n)) { 
		d->doubleLists[n] = new DoublyLinkedList; 
		LINIT(d->doubleLists[n]); 
	}
	return d->doubleLists[n];
}

static StackData* stack(ProgramData* d, const string& n)
{
	if (!d->stacks.count(n)) { 
		d->stacks[n] = new StackData; 
		SINIT(d->stacks[n]); 
	}
	return d->stacks[n];
}

static QueueData* queue(ProgramData* d, const string& n)
{
	if (!d->queues.count(n)) { 
		d->queues[n] = new QueueData; 
		QINIT(d->queues[n]); 
	}
	return d->queues[n];
}

static DoubleQueueData* doubleQueue(ProgramData* d, const string& n)
{
	if (!d->doubleQueues.count(n)) { 
		d->doubleQueues[n] = new DoubleQueueData; 
		DQINIT(d->doubleQueues[n]); 
	}
	return d->doubleQueues[n];
}

static void printType(ProgramData* d, const string& type)
{
	for (map<string, MArray*>::iterator i=d->arrays.begin(); i!=d->arrays.end(); ++i)
		if (type=="M" || type=="ALL") { 
			cout << i->first << ": "; 
			MPRINT(i->second); 
		}
	for (map<string, FNode*>::iterator i=d->singleLists.begin(); i!=d->singleLists.end(); ++i)
		if (type=="F" || type=="ALL") { 
			cout << i->first << ":\n"; 
			FPRINT(i->second); FPRINTREVERSE(i->second);
		}
	for (map<string, DoublyLinkedList*>::iterator i=d->doubleLists.begin(); i!=d->doubleLists.end(); ++i)
		if (type=="L" || type=="ALL") { 
			cout << i->first << ":\n"; 
			LPRINT(i->second); LPRINTREVERSE(i->second); 
		}
	for (map<string, StackData*>::iterator i=d->stacks.begin(); i!=d->stacks.end(); ++i)
		if (type=="S" || type=="ALL") { 
			cout << i->first << ":\n"; 
			SPRINT(i->second); 
		}
	for (map<string, QueueData*>::iterator i=d->queues.begin(); i!=d->queues.end(); ++i)
		if (type=="Q" || type=="ALL") { 
			cout << i->first << ":\n"; 
			QPRINT(i->second); 
		}
	for (map<string, DoubleQueueData*>::iterator i=d->doubleQueues.begin(); i!=d->doubleQueues.end(); ++i)
		if (type=="D" || type=="ALL") { 
			cout << i->first << ":\n"; 
			DQPRINT(i->second); 
		}
	for (map<string, BSTNode*>::iterator i=d->binaryTrees.begin(); i!=d->binaryTrees.end(); ++i)
		if (type=="B" || type=="ALL") { 
			cout << i->first << ": "; 
			BSTINORDER(i->second); cout << endl; 
		}
	for (map<string, RBNode*>::iterator i=d->trees.begin(); i!=d->trees.end(); ++i)
		if (type=="T" || type=="ALL") { 
			cout << i->first << ":\n"; 
			TPRINT(i->second); 
		}
}

bool RunCommand(ProgramData* d, const vector<string>& w, bool& changed, string& error)
{
	if (w.empty()) { 
		error = "Пустая команда."; 
		return false;
	}
	const string& c=w[0];
	if (c=="HELP") { 
		RunHelp(); 
		return true; 
	}
	if (c=="EXIT") return true;
	if (c=="PRINT")
	{
		if (w.size()==1) printType(d,"ALL");
		else if (w.size()==2 && (w[1]=="ALL" || w[1]=="M" || w[1]=="F" || w[1]=="L" || w[1]=="S" || w[1]=="Q" || w[1]=="D" || w[1]=="B" || w[1]=="T")) printType(d,w[1]);
		else if (w.size()==2 && d->arrays.count(w[1])) MPRINT(d->arrays[w[1]]);
		else if (w.size()==2 && d->singleLists.count(w[1])) FPRINT(d->singleLists[w[1]]);
		else if (w.size()==2 && d->doubleLists.count(w[1])) LPRINT(d->doubleLists[w[1]]);
		else if (w.size()==2 && d->stacks.count(w[1])) SPRINT(d->stacks[w[1]]);
		else if (w.size()==2 && d->queues.count(w[1])) QPRINT(d->queues[w[1]]);
		else if (w.size()==2 && d->doubleQueues.count(w[1])) DQPRINT(d->doubleQueues[w[1]]);
		else if (w.size()==2 && d->binaryTrees.count(w[1])) { 
			BSTINORDER(d->binaryTrees[w[1]]); 
			cout << endl; 
		}
		else if (w.size()==2 && d->trees.count(w[1])) TPRINT(d->trees[w[1]]);
		else { 
			error="Неизвестная структура для PRINT."; 
			return false; 
		}
		return true;
	}
	if (c=="MPUSH" && w.size()==3) { 
		MPUSH(getArray(d,w[1]),w[2]); 
		changed=true; 
	}
	else if (c=="MINSERT" && w.size()==4) { 
		MINSERT(getArray(d,w[1]),atoi(w[2].c_str()),w[3]); 
		changed=true; 
	}
	else if (c=="MGET" && w.size()==3) cout << "-> " << MGET(getArray(d,w[1]),atoi(w[2].c_str())) << endl;
	else if (c=="MDEL" && w.size()==3) { 
		MDEL(getArray(d,w[1]),atoi(w[2].c_str())); 
		changed=true; 
	}
	else if ((c=="MSET" || c=="MREPLACE") && w.size()==4) { 
		MREPLACE(getArray(d,w[1]),atoi(w[2].c_str()),w[3]); 
		changed=true; 
	}
	else if ((c=="MLEN" || c=="MLENGTH") && w.size()==2) cout << "-> " << MLENGTH(getArray(d,w[1])) << endl;
	else if (c=="FPUSHHEAD" && w.size()==3) { 
		FPUSHHEAD(singleList(d,w[1]),w[2]); 
		changed=true; 
	}
	else if (c=="FPUSHTAIL" && w.size()==3) { 
		FPUSHTAIL(singleList(d,w[1]),w[2]); 
		changed=true; 
	}
	else if (c=="FPUSHAFTER" && w.size()==4) { 
		FNode* n=FGET(singleList(d,w[1]),w[2]); 
		if(n) FINSERTAFTER(n,w[3]); 
		else { 
			error="Опорный элемент не найден."; 
			return false; 
		} 
		changed=true; 
	}
	else if (c=="FPUSHBEFORE" && w.size()==4) { 
		FNode*& h=singleList(d,w[1]); 
		FINSERTBEFORE(h,FGET(h,w[2]),w[3]); 
		changed=true; 
	}
	else if (c=="FDELHEAD" && w.size()==2) { 
		FDELHEAD(singleList(d,w[1])); 
		changed=true; 
	}
	else if (c=="FDELTAIL" && w.size()==2) { 
		FDELTAIL(singleList(d,w[1])); 
		changed=true; 
	}
	else if (c=="FDELAFTER" && w.size()==3) { 
		FNode* n=FGET(singleList(d,w[1]),w[2]); 
		if(!n || !n->nextEl) { 
			error="Элемент не найден."; 
			return false; 
		} 
		FDELAFTER(n); 
		changed=true; 
	}
	else if (c=="FDELBEFORE" && w.size()==3) { 
		FNode*& h=singleList(d,w[1]); 
		FDELBEFORE(h,FGET(h,w[2])); 
		changed=true; 
	}
	else if ((c=="FDELVAL" || c=="FDELVALUE") && w.size()==3) { 
		FDELVALUE(singleList(d,w[1]),w[2]); 
		changed=true; 
	}
	else if (c=="FGET" && w.size()==3) { 
		FNode* n=FGET(singleList(d,w[1]),w[2]); 
		cout << "-> " << (n?n->data:"FALSE") << endl; 
	}
	else if (c=="LPUSHHEAD" && w.size()==3) { 
		LPUSHHEAD(doubleList(d,w[1]),w[2]); 
		changed=true; 
	}
	else if (c=="LPUSHTAIL" && w.size()==3) { 
		LPUSHTAIL(doubleList(d,w[1]),w[2]); 
		changed=true; 
	}
	else if (c=="LPUSHAFTER" && w.size()==4) { 
		DoublyLinkedList* l=doubleList(d,w[1]); 
		LINSERTAFTER(l,LGET(l,w[2]),w[3]); 
		changed=true; 
	}
	else if (c=="LPUSHBEFORE" && w.size()==4) { 
		DoublyLinkedList* l=doubleList(d,w[1]); 
		LINSERTBEFORE(l,LGET(l,w[2]),w[3]); 
		changed=true; 
	}
	else if (c=="LDELHEAD" && w.size()==2) { 
		LDELHEAD(doubleList(d,w[1])); 
		changed=true; 
	}
	else if (c=="LDELTAIL" && w.size()==2) { 
		LDELTAIL(doubleList(d,w[1])); 
		changed=true; 
	}
	else if (c=="LDELAFTER" && w.size()==3) { 
		DoublyLinkedList* l=doubleList(d,w[1]); 
		LNode* n=LGET(l,w[2]); 
		if(!n||!n->nextEl){
			error="Элемент не найден.";
			return false;
		} 
		LDELNODE(l,n->nextEl); 
		changed=true; 
	}
	else if (c=="LDELBEFORE" && w.size()==3) { 
		DoublyLinkedList* l=doubleList(d,w[1]); 
		LNode* n=LGET(l,w[2]); 
		if(!n||!n->prevEl){
			error="Элемент не найден.";
			return false;
		} 
		LDELNODE(l,n->prevEl); 
		changed=true; }
	else if ((c=="LDELVAL" || c=="LDELVALUE") && w.size()==3) { 
		LDELVALUE(doubleList(d,w[1]),w[2]); 
		changed=true; 
	}
	else if (c=="LGET" && w.size()==3) { 
		LNode* n=LGET(doubleList(d,w[1]),w[2]); 
		cout << "-> " << (n?n->data:"FALSE") << endl; 
	}
	else if (c=="SPUSH" && w.size()==3) { 
		SPUSH(stack(d,w[1]),w[2]); 
		changed=true; 
	}
	else if (c=="SPOP" && w.size()==2) { 
		cout << "-> " << SPOP(stack(d,w[1])) << endl; 
		changed=true; 
	}
	else if (c=="QPUSH" && w.size()==3) { 
		QPUSH(queue(d,w[1]),w[2]); 
		changed=true; 
	}
	else if (c=="QPOP" && w.size()==2) { 
		cout << "-> " << QPOP(queue(d,w[1])) << endl; 
		changed=true; 
	}
	else if (c=="QGET" && w.size()==2) cout << "-> " << QGET(queue(d,w[1])) << endl;
	else if (c=="DQPUSH" && w.size()==3) { 
		DQPUSH(doubleQueue(d,w[1]),w[2]); 
		changed=true; 
	}
	else if (c=="DQPOP" && w.size()==2) { 
		cout << "-> " << DQPOP(doubleQueue(d,w[1])) << endl; 
		changed=true; 
	}
	else if (c=="DQGET" && w.size()==2) cout << "-> " << DQGET(doubleQueue(d,w[1])) << endl;
	else if ((c=="BINSERT" || c=="BDEL" || c=="BGET" || c=="TINSERT" || c=="TDEL" || c=="TGET" || c=="ISMEMBER") && w.size()==3)
	{
		int key=atoi(w[2].c_str());
		if (c=="BINSERT") { 
			d->binaryTrees[w[1]]=BSTINSERT(d->binaryTrees[w[1]],key); 
			changed=true; 
		}
		else if (c=="BDEL") { 
			d->binaryTrees[w[1]]=BSTDELETE(d->binaryTrees[w[1]],key); 
			changed=true; 
		}
		else if (c=="BGET") cout << "-> " << (BSTSEARCH(d->binaryTrees[w[1]],key)?"TRUE":"FALSE") << endl;
		else if (c=="TINSERT") { 
			d->trees[w[1]]=TINSERT(d->trees[w[1]],key); 
			changed=true; 
		}
		else if (c=="TDEL") { 
			d->trees[w[1]]=TDEL(d->trees[w[1]],key); 
			changed=true; 
		}
		else cout << "-> " << (TGET(d->trees[w[1]],key)?"TRUE":"FALSE") << endl;
	}
	else { 
		error="Неизвестная команда или неверное число аргументов."; 
		return false; 
	}
	return true;
}

void RunHelp()
{
	cout << "M: MPUSH MINSERT MGET MDEL MSET MLEN; "
	     << "F: FPUSHHEAD FPUSHTAIL FPUSHAFTER FPUSHBEFORE "
		 << "FDELHEAD FDELTAIL FDELAFTER FDELBEFORE FDELVAL FGET" << endl;
	cout << "L: LPUSHHEAD LPUSHTAIL LPUSHAFTER LPUSHBEFORE LDELHEAD "
		 << "LDELTAIL LDELAFTER LDELBEFORE LDELVAL LGET; S: SPUSH SPOP" << endl;
	cout << "Q: QPUSH QPOP QGET; D: DQPUSH DQPOP DQGET; "
	     << "B: BINSERT BDEL BGET; T: TINSERT TDEL TGET; PRINT; EXIT" << endl;
}
