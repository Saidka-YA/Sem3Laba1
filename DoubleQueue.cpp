#include "DoubleQueue.h"

void DQINIT(DoubleQueueData* q)
{
	q->head = nullptr;
	q->tail = nullptr;
}

void DQPUSH(DoubleQueueData* q, const string& str)
{
	DoubleQueueNode* newEl = new DoubleQueueNode{str, nullptr, q->tail};
	if (q->tail == nullptr)
	{
		q->head = newEl;
		q->tail = newEl;
	}
	else
	{
		q->tail->nextEl = newEl;
		q->tail = newEl;
	}
}

string DQPOP(DoubleQueueData* q)
{
	if (q->head == nullptr)
	{
		cout << "Очередь пуста удалять нечего!\n";
		return "";
	}
	DoubleQueueNode* tmp = q->head;
	string value = tmp->data;
	q->head = q->head->nextEl;
	if (q->head == nullptr)
	{
		q->tail = nullptr;
	}
	else
	{
		q->head->prevEl = nullptr;
	}
	delete tmp;
	return value;
}

void DQPRINT(DoubleQueueData* q)
{
	if (q->head == nullptr)
	{
		cout << "Очередь пуста выводить нечего!\n";
		return;
	}
	DoubleQueueNode* curr = q->head;
	while (curr != nullptr)
	{
		cout << curr->data << endl;
		curr = curr->nextEl;
	}
}

bool DQISEMPTY(DoubleQueueData* q)
{
	return q->head == nullptr;
}

string DQGET(DoubleQueueData* q)
{
	if (DQISEMPTY(q))
	{
		cout << "Очередь пуста читать нечего!\n";
		return "";
	}
	return q->head->data;
}

int DQLENGTH(DoubleQueueData* q)
{
	int length = 0;
	DoubleQueueNode* curr = q->head;
	while (curr != nullptr)
	{
		length++;
		curr = curr->nextEl;
	}
	return length;
}

void DQCLEAR(DoubleQueueData* q)
{
	while (!DQISEMPTY(q))
	{
		DQPOP(q);
	}
}
