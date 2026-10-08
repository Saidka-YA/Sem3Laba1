#include "DoubleLinearList.h"


void LPRINT(DoublyLinkedList* dblLst)
{
	if (dblLst->head == nullptr)
	{
		cout << "Список пуст выводить нечего!\n";
		return;
	}
	LNode* curr = dblLst->head;
	while (curr != nullptr)
	{
		cout  << curr->data << endl;
		curr = curr->nextEl;
	}
}

void LINSERTAFTER(DoublyLinkedList* dblLst, LNode* El, const string& str)
{
	if (El == nullptr)
	{
		cout << "Ошибка\n";
		return;
	}
	LNode* newEl = new LNode{str, nullptr, nullptr};
	newEl->nextEl = El->nextEl;
	newEl->prevEl = El;
	if (El->nextEl != nullptr) El->nextEl->prevEl = newEl;
	else dblLst->tail = newEl;
	El->nextEl = newEl;
}

void LPUSHHEAD(DoublyLinkedList* dblLst, const string& str)
{
	LNode* newEl = new LNode{str, nullptr, nullptr};
	if (dblLst->head == nullptr)
	{
		dblLst->head = newEl;
		dblLst->tail = newEl;
	}
	else 
	{
		newEl->nextEl = dblLst->head;
		dblLst->head->prevEl = newEl;
		dblLst->head = newEl;
	}
}

void LDELNODE(DoublyLinkedList* dblLst, LNode* El)
{
	if (El == nullptr)
	{
		cout << "Ошибка\n";
		return;
	}
	if (El->prevEl == nullptr) dblLst->head = El->nextEl;
	else El->prevEl->nextEl = El->nextEl;
	if  (El->nextEl == nullptr) dblLst->tail = El->prevEl;
	else El->nextEl->prevEl = El->prevEl;
	delete El;
}

void LINIT(DoublyLinkedList* dblLst)
{
	dblLst->head = nullptr;
	dblLst->tail = nullptr;
}

void LPUSHTAIL(DoublyLinkedList* dblLst, const string& str)
{
	LNode* newEl = new LNode{str, nullptr, dblLst->tail};
	if (dblLst->tail == nullptr)
	{
		dblLst->head = newEl;
		dblLst->tail = newEl;
	}
	else
	{
		dblLst->tail->nextEl = newEl;
		dblLst->tail = newEl;
	}
}

void LINSERTBEFORE(DoublyLinkedList* dblLst, LNode* El, const string& str)
{
	if (El == nullptr)
	{
		cout << "Элемент не найден!\n";
		return;
	}
	if (El == dblLst->head)
	{
		LPUSHHEAD(dblLst, str);
		return;
	}
	LNode* newEl = new LNode{str, El, El->prevEl};
	El->prevEl->nextEl = newEl;
	El->prevEl = newEl;
}

void LDELHEAD(DoublyLinkedList* dblLst)
{
	if (dblLst->head == nullptr) return;
	LDELNODE(dblLst, dblLst->head);
}

void LDELTAIL(DoublyLinkedList* dblLst)
{
	if (dblLst->tail == nullptr) return;
	LDELNODE(dblLst, dblLst->tail);
}

LNode* LGET(DoublyLinkedList* dblLst, const string& str)
{
	LNode* curr = dblLst->head;
	while (curr != nullptr)
	{
		if (curr->data == str) return curr;
		curr = curr->nextEl;
	}
	return nullptr;
}

void LDELVALUE(DoublyLinkedList* dblLst, const string& str)
{
	LNode* El = LGET(dblLst, str);
	if (El == nullptr)
	{
		cout << "Элемент не найден!\n";
		return;
	}
	LDELNODE(dblLst, El);
}

void LPRINTREVERSE(DoublyLinkedList* dblLst)
{
	LNode* curr = dblLst->tail;
	while (curr != nullptr)
	{
		cout << curr->data << endl;
		curr = curr->prevEl;
	}
}
