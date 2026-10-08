#ifndef QUEUE_H
#define QUEUE_H
#pragma once
#include <iostream>
#include <string>
using namespace std;

struct QueueNode
{
	string data;
	QueueNode*  nextEl;
};

struct QueueData
{
	QueueNode* head;
	QueueNode* tail;
};

void QINIT(QueueData* q);
void QPUSH(QueueData* q, const string& str);
string QPOP(QueueData* q);
void QPRINT(QueueData* q);
void QPRINTADDR(QueueData* q);
bool QISEMPTY(QueueData* q);
string QGET(QueueData* q);
int QLENGTH(QueueData* q);
void QCLEAR(QueueData* q);

#endif