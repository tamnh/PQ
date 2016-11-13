//
// Created by Nguyen Hung Tam on 11/9/16.
//
#include "BTreeNode.h"

#include <algorithm>

using namespace std;

BTreeNode::BTreeNode() {
    parent = vector<BTreeNode>();
    children = vector<BTreeNode>();
}

BTreeNode::BTreeNode(BTreeNode leftNode, BTreeNode rightNode) {
    if (leftNode.isLeaf()) {
        key = leftNode.key;
        for(int i=0; i<rightNode.key.size(); i++) {
            key.push_back(rightNode.key[i]);
        }
    } else {
        children = leftNode.children;
        for(int i=0; i<rightNode.children.size(); i++) {
            children.push_back(rightNode.children[i]);
        }
        initKey();
    }
}

bool BTreeNode::isRoot() {
    return parent.empty();
}


bool BTreeNode::isLeaf() {
    return children.empty();
}


bool BTreeNode::is_too_big() {
    if (isLeaf()) {
        return key.size() >= 4 * B;
    } else {
        return children.size() >= 4 * B;
    }
}


bool BTreeNode::is_too_small() {
    if (isLeaf()) {
        return key.size() < B;
    } else {
        return children.size() < B;
    }
}


bool BTreeNode::isEmpty() {
    return key.empty() && children.empty();
}


void BTreeNode::initKey() {
    key.clear();
    for(int i=0; i<children.size()-1; i++) {
        key.push_back(children[i+1].key[0]);
    }
}



pii BTreeNode::extractMin() {
    if (isEmpty()) {
        return make_pair(-1, -1);
    }

    if (isLeaf()) {
        pii result = key[0];
        key.erase(key.begin());
        return result;
    } else {
        pii result = children[0].extractMin();
        if (children[0].isEmpty()) {
            children.erase(children.begin());
            key.erase(key.begin());
        }
        return result;
    }
}


pair<BTreeNode, BTreeNode> BTreeNode::split() {
    BTreeNode leftNode, rightNode;

    if (isLeaf()) {
        for(int i=0; i<key.size(); i++) {
            if (i < key.size() / 2) {
                leftNode.key.push_back(key[i]);
            } else {
                rightNode.key.push_back(key[i]);
            }
        }
    }
    else {
        for(int i=0; i<children.size(); i++) {
            if (i < children.size() / 2) {
                leftNode.children.push_back(children[i]);
            } else {
                rightNode.children.push_back(children[i]);
            }
        }

        leftNode.initKey();
        rightNode.initKey();
    }

    return make_pair(leftNode, rightNode);
}


void BTreeNode::deleteNode(int priority, int new_key) {
    if (isLeaf()) {
        pii entry = make_pair(priority, new_key );
        vector<pii>::iterator it = lower_bound(key.begin(), key.end(), entry);

        //if found entry
        if ((*it) == entry) {
            key.erase(it);
        }
    }

    else {
        pii entry = make_pair(priority, new_key + 1);
        vector<pii>::iterator it = lower_bound(key.begin(), key.end(), entry);
        int pos = it - key.begin();
        children[pos].deleteNode(priority, new_key);

        if (children[pos].is_too_small()) {
            if (pos == 0) {
                if (children.size() == 1) return;
                else pos = 1;
            }

            BTreeNode new_node = BTreeNode(children[pos-1], children[pos]);
            children.erase(children.begin() + pos - 1);
            children.erase(children.begin() + pos - 1);

            if (new_node.is_too_big()) {
                // split again
                pair<BTreeNode, BTreeNode> p = new_node.split();
                BTreeNode leftNode = p.first;
                BTreeNode rightNode = p.second;
                children.insert(children.begin() + pos - 1, leftNode);
                children.insert(children.begin() + pos, rightNode);
            } else {
                children.insert(children.begin() + pos - 1, new_node);
            }
        }

        initKey();
    }
}


void BTreeNode::insertNode(int priority, int new_key) {

    if (isLeaf()) {
        pii entry = make_pair(priority, new_key);
        vector<pii>::iterator it = lower_bound(key.begin(), key.end(), entry);
        key.insert(it, entry);
    }

    else {
        pii entry = make_pair(priority, new_key);
        vector<pii>::iterator it = lower_bound(key.begin(), key.end(), entry);
        int pos = it - key.begin();
        children[pos].insertNode(priority, new_key);
        //handling node splitting and merging
        if (children[pos].is_too_big()) {
            pair<BTreeNode, BTreeNode> p = children[pos].split();
            children.erase(children.begin() + pos);
            children.insert(children.begin() + pos, p.first);
            children.insert(children.begin() + pos + 1, p.second);
            initKey();
        }
    }
}



