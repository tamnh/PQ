//
// Created by Nguyen Hung Tam on 11/9/16.
//
#ifndef BTREENODE_H
#define BTREENODE_H

#include <vector>
#include <cstdio>

using namespace std;

typedef pair<int, int> pii;
const int B = 64;

class BTreeNode {

public:
    int counter;

    vector<pii> key;
    vector<BTreeNode> children;
    vector<BTreeNode> parent;

    BTreeNode();
    // BTreeNode(BTreeNode);
    BTreeNode(BTreeNode, BTreeNode);

    bool isRoot();
    bool isLeaf();
    bool is_too_big();
    bool is_too_small();
    bool isEmpty();

    void initKey();
    void insertNode(int, int);
    void deleteNode(int, int);
    pii extractMin();

    pair<BTreeNode, BTreeNode> split();
};

#endif //PROJECT_BTREENODE_H
