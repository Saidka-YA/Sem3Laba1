#ifndef DOUBLEQUEUE_H
#define DOUBLEQUEUE_H
#pragma once
#include <iostream>
#include <string>
using namespace std;

struct DoubleQueueNode
{
	string data;
	DoubleQueueNode* nextEl;
	DoubleQueueNode* prevEl;
};

struct DoubleQueueData
{
	DoubleQueueNode* head;
	DoubleQueueNode* tail;
};

void DQINIT(DoubleQueueData* q);
void DQPUSH(DoubleQueueData* q, const string& str);
string DQPOP(DoubleQueueData* q);
void DQPRINT(DoubleQueueData* q);
bool DQISEMPTY(DoubleQueueData* q);
string DQGET(DoubleQueueData* q);
int DQLENGTH(DoubleQueueData* q);
void DQCLEAR(DoubleQueueData* q);

#endif