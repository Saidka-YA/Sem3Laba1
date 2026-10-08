#ifndef BST_TREE_H
#define BST_TREE_H
#pragma once
#include <iostream>
using namespace std;

struct BSTNode
{
	int data;
	BSTNode* left;
	BSTNode* right;
	BSTNode* parent;
	BSTNode(int k) {data = k; left = right = parent = nullptr;}
};

void BSTINORDER(BSTNode* node);
void BSTPREORDER(BSTNode* node);
void BSTPOSTORDER(BSTNode* node);
BSTNode* BSTINSERT(BSTNode* node, int data);
int BSTSEARCH(BSTNode* root, int data);
BSTNode* BSTFINDMIN(BSTNode* value);
BSTNode* BSTDELETE(BSTNode* node, int data);


#endif