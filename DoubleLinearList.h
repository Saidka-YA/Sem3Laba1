#ifndef DOUBLELINEARLIST_H
#define DOUBLELINEARLIST_H
#pragma once
#include <iostream>
#include <string>
using namespace std;

struct LNode
{
	string data;
	LNode* nextEl;
	LNode* prevEl;
};

struct DoublyLinkedList
{
	LNode* head;
	LNode* tail;
};

void LPRINT(DoublyLinkedList* dblLst);
void LINSERTAFTER(DoublyLinkedList* dblLst, LNode* El, const string& str);
void LPUSHHEAD(DoublyLinkedList* dblLst, const string& str);
void LDELNODE(DoublyLinkedList* dblLst, LNode* El);
void LINIT(DoublyLinkedList* dblLst);
void LPUSHTAIL(DoublyLinkedList* dblLst, const string& str);
void LINSERTBEFORE(DoublyLinkedList* dblLst, LNode* El, const string& str);
void LDELHEAD(DoublyLinkedList* dblLst);
void LDELTAIL(DoublyLinkedList* dblLst);
LNode* LGET(DoublyLinkedList* dblLst, const string& str);
void LDELVALUE(DoublyLinkedList* dblLst, const string& str);
void LPRINTREVERSE(DoublyLinkedList* dblLst);

#endif