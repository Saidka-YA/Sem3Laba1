#ifndef RB_TREE_H
#define RB_TREE_H
#pragma once
#include <iostream>
using namespace std;

enum Color {RED, BLACK};

struct RBNode {
    int key;
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;
    RBNode(int k) {
        key = k; 
        left = right = parent = nullptr; 
        color = RED;}
};

RBNode* RBROTATERIGHT(RBNode* root, RBNode* value);
RBNode* RBROTATELEFT(RBNode* root, RBNode* value);
RBNode* RBFIXINSERT(RBNode* root, RBNode* element);
RBNode* TINSERT(RBNode* root, int key);
RBNode* TGET(RBNode* root, int key);
void TPRINT(RBNode* value);
RBNode* RBFINDMIN(RBNode* value);
RBNode* RBTRANSPLANT(RBNode* root, RBNode* oldValue, RBNode* newValue);
Color RBGETCOLOR(RBNode* value);
RBNode* RBFIXDELETE(RBNode* root, RBNode* x, RBNode* parent);
RBNode* TDEL(RBNode* root, int key);

#endif