//
// Created by Nguyen Hung Tam on 11/10/16.
//

#include "PQBTree.h"

PQBTree::PQBTree() {
    root = BTreeNode();
}


bool PQBTree::isEmpty() {
    return root.isEmpty();
}


void PQBTree::insertNode(int priority, int key) {
    if (keyMap.count(key)) {
        if (keyMap[key] <= priority) {
            return;
        } else {
            deleteNode(priority, key);
        }
    }

    keyMap[key] = priority;

    root.insertNode(priority, key);

    if (root.is_too_big()) {
        pair<BTreeNode, BTreeNode> p = root.split();
        BTreeNode new_root = BTreeNode();
        new_root.children.push_back(p.first);
        new_root.children.push_back(p.second);
        root = new_root;
    }
}

void PQBTree::deleteNode(int priority, int key) {
    root.deleteNode(priority, key);
    if (root.children.size() == 1) {
        root = root.children[0];
    }
}

pair<int, int> PQBTree::extractMin() {
    pair<int, int> result = root.extractMin();
    if (root.children.size() == 1) {
        root = root.children[0];
    }
    return result;
}

