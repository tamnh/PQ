//
// Created by Nguyen Hung Tam on 11/10/16.
//

#ifndef PROJECT_PQBTREE_H
#define PROJECT_PQBTREE_H

#include "BTreeNode.h"
#include <map>

class PQBTree {
    public:

    BTreeNode root;
    map<int, int> keyMap;

    PQBTree();
    bool isEmpty();
    void insertNode(int, int);
    void deleteNode(int, int);
    pair<int, int> extractMin();

};
#endif //PROJECT_PQBTREE_H
