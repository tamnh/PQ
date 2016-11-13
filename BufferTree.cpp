//
// Created by thbl2 on 11/13/2016.
//

#include "BufferTree.h"

BufferTree::BufferTree(unsigned blockSize) {
    B = blockSize;
    root = new BufferTreeNode(B, B*4, B, B*2, B*2);
}

bool BufferTree::isEmpty() {
    return root->elements.empty();
}

void BufferTree::insert(unsigned key, unsigned priority) {
    root->add(key, priority);
    if (root->isFull()) {
        BufferTreeNode* newRoot = new BufferTreeNode(2, 4, B, B*2, B*2);
        newRoot->elements.push_back(root->getMax());
        newRoot->children.push_back(root);
        newRoot->smallElements = root->smallElements;
        newRoot->buffer = root->buffer;
        newRoot->split(0);
        root = newRoot;
    }
}

void BufferTree::remove(unsigned key) {
    root->remove(key);
}

Element BufferTree::deleteMin() {
    return root->deleteMin();
}
