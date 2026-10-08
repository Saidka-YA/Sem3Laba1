#include "Queue.h"

void QINIT(QueueData* q)
{
	q->head = nullptr;
	q->tail = nullptr;
}

void QPUSH(QueueData* q, const string& str)
{
	QueueNode* newEl = new QueueNode{str, nullptr};
	if (q->head == nullptr)
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

string QPOP(QueueData* q)
{
	if (q->head == nullptr)
	{
		cout << "Очередь пуста удалять нечего!\n";
		return "";
	}
	QueueNode* tmp = q->head;
	string value = tmp->data;
	q->head = q->head->nextEl;
	if (q->head == nullptr)
	{
		q->tail = nullptr;
	}
	delete tmp;
	return value;
}

void QPRINT(QueueData* q)
{
	if (q->head == nullptr)
	{
		cout << "Очередь пуста выводить нечего!\n";
		return;
	}
	QueueNode* curr = q->head;
	while (curr != nullptr)
	{
		cout << curr->data << endl;
		curr = curr->nextEl;
	}
}

void QPRINTADDR(QueueData* q)
{
	if (q->head == nullptr)
	{
		cout << "Очередь пуста нечего выводить!\n";
		return;
	}
	QueueNode* curr = q->head;
	cout << "Указатель на голову очереди: " << q->head << endl;
	while (curr != nullptr)
	{
		cout << "Aдрес узла: " << curr << endl
			<< "Значение: " << curr->data << endl
			<< "Указывает на: " << curr->nextEl << endl;
		curr = curr->nextEl;
	}
	cout << "Указатель на хвост очереди: " << q->tail << endl;
}

bool QISEMPTY(QueueData* q)
{
	return q->head == nullptr;
}

string QGET(QueueData* q)
{
	if (QISEMPTY(q))
	{
		cout << "Очередь пуста читать нечего!\n";
		return "";
	}
	return q->head->data;
}

int QLENGTH(QueueData* q)
{
	int length = 0;
	QueueNode* curr = q->head;
	while (curr != nullptr)
	{
		length++;
		curr = curr->nextEl;
	}
	return length;
}

void QCLEAR(QueueData* q)
{
	while (!QISEMPTY(q))
	{
		QPOP(q);
	}
}
