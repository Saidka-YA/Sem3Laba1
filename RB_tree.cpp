#include "RB_tree.h"

RBNode* RBROTATERIGHT(RBNode* root, RBNode* value) {
    RBNode* tmp = value->left;
    RBNode* parent = value->parent;
    value->left = tmp->right;
    
    if (tmp->right != nullptr) {
        tmp->right->parent = value;
    }

    tmp->parent = parent;
    if (parent == nullptr) {
        root = tmp;
    } else if (value == parent->left) {
        parent->left = tmp;
    } else {
        parent->right = tmp;
    }

    tmp->right = value;
    value->parent = tmp;
    return root;
}

RBNode* RBROTATELEFT(RBNode* root, RBNode* value) {
    RBNode* tmp = value->right;
    RBNode* parent = value->parent;
    value->right = tmp->left;

    if (tmp->left != nullptr) {
        tmp->left->parent = value;
    }
    tmp->parent = parent;
    if (parent == nullptr) {
        root = tmp;
    } else if (value == parent->left) {
        parent->left = tmp;
    } else {
        parent->right = tmp;
    }

    tmp->left = value;
    value->parent = tmp;
    return root;
}

RBNode* RBFIXINSERT(RBNode* root, RBNode* element) {
    while (element->parent != nullptr && element->parent->color == RED) {   
        if (element->parent == element->parent->parent->left) {
            RBNode* uncle = element->parent->parent->right;
            if (uncle!= nullptr && uncle->color == RED) {               
                element->parent->color = BLACK;
                uncle->color = BLACK;
                element->parent->parent->color = RED;
                element = element->parent->parent;
            } else {
                if (element == element->parent->right) {
                    element = element->parent;
                    root = RBROTATELEFT(root, element);                        
                }
                element->parent->color = BLACK;
                element->parent->parent->color = RED;
                root = RBROTATERIGHT(root, element->parent->parent);
            }
        } else {
            if (element->parent->parent->left != nullptr && element->parent->parent->left->color == RED) {
                element->parent->color = BLACK;
                element->parent->parent->left->color = BLACK;
                element->parent->parent->color = RED;
                element = element->parent->parent;
            } else { 
                if (element == element->parent->left) {
                    element = element->parent;
                    root = RBROTATERIGHT(root, element);
                }
                element->parent->color = BLACK;
                element->parent->parent->color = RED;
                root = RBROTATELEFT(root, element->parent->parent);
            }
        }
    } 
    root->color = BLACK;
    return root;
}

RBNode* TINSERT(RBNode* root, int key) {
    if (root == nullptr) {
        RBNode* tmp = new RBNode(key);
        tmp->color = BLACK;
        return tmp;
    }
    RBNode* curr = root;
    RBNode* parent = nullptr;
    while (curr != nullptr){
        parent = curr;
        if (key < curr->key) {curr = curr->left;}
        else if (key > curr->key) {curr = curr->right;}
        else {return root;}
    }
    RBNode* newEl = new RBNode(key);
    newEl->parent = parent;

    if (key < parent->key) {parent->left = newEl;}
    else {parent->right = newEl;}
    root = RBFIXINSERT(root, newEl);
    return root;
}

RBNode* TGET(RBNode* root, int key) {
    RBNode* curr = root;

    while (curr != nullptr) {
        if (key < curr->key) {curr = curr->left;}
        else if (key > curr->key) {curr = curr->right;}
        else {return curr;}
    }
    return nullptr;
}

void TPRINT(RBNode* value) {
    if (value == nullptr) return;
    TPRINT(value->left);
    cout << value->key << ' ' << value->color << endl;
    TPRINT(value->right);
}

RBNode* RBFINDMIN(RBNode* value) {
    while (value->left != nullptr) {
        value = value->left;
    }
    return value;
}

RBNode* RBTRANSPLANT(RBNode* root, RBNode* oldValue, RBNode* newValue) {
    if (oldValue->parent == nullptr) {
        root = newValue;
    } else if (oldValue == oldValue->parent->left) {
        oldValue->parent->left = newValue;
    } else {
        oldValue->parent->right = newValue;
    }

    if (newValue != nullptr) {
        newValue->parent = oldValue->parent;
    }
    return root;
}
Color RBGETCOLOR(RBNode* value) {
    if (value == nullptr) {
        return BLACK;
    }
    return value->color;
}
RBNode* RBFIXDELETE(RBNode* root, RBNode* x, RBNode* parent) {
    while (x != root && RBGETCOLOR(x) == BLACK && parent != nullptr) {
        if (x == parent->left) {
            RBNode* sibling = parent->right;
            if (RBGETCOLOR(sibling) == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                root = RBROTATELEFT(root, parent);
                sibling = parent->right;
            }
            if (sibling == nullptr) {
                x = parent;
                parent = x->parent;
                continue;
            }
            if (RBGETCOLOR(sibling->left) == BLACK && RBGETCOLOR(sibling->right) == BLACK) {
                sibling->color = RED;
                x = parent;
                parent = x->parent;
            } else {
                if (RBGETCOLOR(sibling->right) == BLACK) {
                    if (sibling->left != nullptr) {
                        sibling->left->color = BLACK;
                    }
                    sibling->color = RED;
                    root = RBROTATERIGHT(root, sibling);
                    sibling = parent->right;
                }
                sibling->color = parent->color;
                parent->color = BLACK;

                if (sibling->right != nullptr) {
                    sibling->right->color = BLACK;
                }
                root = RBROTATELEFT(root, parent);
                x = root;
                parent = nullptr;
            }
        } else {
            RBNode* sibling = parent->left;
            if (sibling == nullptr) {
                x = parent;
                parent = x->parent;
                continue;
            }
            if (RBGETCOLOR(sibling) == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                root = RBROTATERIGHT(root, parent);
                sibling = parent->left;
            }
            if (sibling == nullptr) {
                x = parent;
                parent = x->parent;
                continue;
            }
            if (RBGETCOLOR(sibling->right) == BLACK && RBGETCOLOR(sibling->left) == BLACK) {
                sibling->color = RED;
                x = parent;
                parent = x->parent;
            } else {
                if (RBGETCOLOR(sibling->left) == BLACK) {
                    if (sibling->right != nullptr) {
                        sibling->right->color = BLACK;
                    }
                    sibling->color = RED;
                    root = RBROTATELEFT(root, sibling);
                    sibling = parent->left;
                }
                sibling->color = parent->color;
                parent->color = BLACK;

                if (sibling->left != nullptr) {
                    sibling->left->color = BLACK;
                }
                root = RBROTATERIGHT(root, parent);
                x = root;
                parent = nullptr;
            }
            
        }
    }
    if (x != nullptr) {
        x->color = BLACK;
    }
    return root;
}

RBNode* TDEL(RBNode* root, int key) {
    RBNode* z = TGET(root, key);

    RBNode* x = nullptr;
    RBNode* parent = nullptr;

    if (z == nullptr) {
        return root;
    }

    RBNode* y = z;
    Color yOriginalColor = y->color;

    if (z->left == nullptr) {
        x = z->right;
        parent = z->parent;
        root = RBTRANSPLANT(root, z, z->right);
    } else if (z->right == nullptr) {
        x = z->left;
        parent = z->parent;
        root = RBTRANSPLANT(root, z, z->left);
    } else {
        y = RBFINDMIN(z->right);
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == z) {
            parent = y;
            y->parent = z->parent;
            if (x != nullptr) {
                x->parent = y;
            }
        } else {
            parent = y->parent;
            root = RBTRANSPLANT(root, y, y->right);

            y->right = z->right;
            y->right->parent = y;
        }
        root = RBTRANSPLANT(root, z, y);

        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    delete z;

    if (yOriginalColor == BLACK) {
        root = RBFIXDELETE(root, x, parent);
    }
    return root;
}