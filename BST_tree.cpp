#include "BST_tree.h"

void BSTINORDER(BSTNode* node)
{
	if (node == nullptr) return;

	BSTINORDER(node->left);
	cout << node->data << ' ';
	BSTINORDER(node->right);
}

void BSTPREORDER(BSTNode* node)
{
	if (node == nullptr) return;

	cout << node->data << ' ';
	BSTPREORDER(node->left);
	BSTPREORDER(node->right);
}

void BSTPOSTORDER(BSTNode* node)
{
	if (node == nullptr) return;

	BSTPOSTORDER(node->left);
	BSTPOSTORDER(node->right);
	cout << node->data << ' ';
}

BSTNode* BSTINSERT(BSTNode* node, int data) {
	if (node == nullptr) return new BSTNode(data);
	if (data < node->data) node->left = BSTINSERT(node->left, data);
	else node->right = BSTINSERT(node->right, data);
	return node;
}
bool BSTSEARCH(BSTNode* root, int data) {
	if (root == nullptr) return false;
	if (root->data == data) return true;
	if (data < root->data) return BSTSEARCH(root->left, data);
	return BSTSEARCH(root->right, data);
}

BSTNode* BSTFINDMIN(BSTNode* value) {
	if (value == nullptr) return nullptr;
	if (value->left == nullptr) return value;
	return BSTFINDMIN(value->left);
}

BSTNode* BSTDELETE(BSTNode* node, int data) {
	if (node == nullptr) return node;
	if (data < node->data) node->left = BSTDELETE(node->left, data);
	else if (data > node->data) node->right = BSTDELETE(node->right, data);
	else {
		if (node->left == nullptr) {
			BSTNode* tmp = node->right;
			delete node;
			return tmp;
		} else if (node->right == nullptr) {
			BSTNode* tmp = node->left;
			delete node;
			return tmp;
		}

		BSTNode* tmp = BSTFINDMIN(node->right);
		node->data = tmp->data;
		node->right = BSTDELETE(node->right, tmp->data);
	}
	return node;
}
