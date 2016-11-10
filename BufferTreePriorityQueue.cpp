//
// Created by thbl2 on 11/10/2016.
//

#include "BufferTreePriorityQueue.h"

BufferTreePriorityQueue::BufferTreePriorityQueue(BufferTreeNode &root) {
    this->root = root;
}

BufferTreePriorityQueue::BufferTreePriorityQueue(unsigned B) {
    this(BufferTreeNode(B));
}

void BufferTreePriorityQueue::setRoot(BufferTreeNode &newRoot) {
    this->root = newRoot;
}

void BufferTreePriorityQueue::remove(unsigned key) {
    this->root.insertOperation(key, DELETE);
}

void BufferTreePriorityQueue::decreaseKey(unsigned key, unsigned newPriority) {
    this->root.insertOperation(key, newPriority*2 + DECREASE_KEY);
}

unsigned BufferTreePriorityQueue::deleteMin() {
    return this->root.deleteMin();
}

