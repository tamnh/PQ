//
// Created by thbl2 on 11/10/2016.
//

#ifndef PROJECT_BUFFERTREEPRIORITYQUEUE_H
#define PROJECT_BUFFERTREEPRIORITYQUEUE_H

#endif //PROJECT_BUFFERTREEPRIORITYQUEUE_H

#include "BufferTreeNode.h"

class BufferTreePriorityQueue {

public:

    BufferTreePriorityQueue(BufferTreeNode &root);

    BufferTreePriorityQueue(unsigned B);

    void setRoot(BufferTreeNode &newRoot);

    void remove(unsigned key);

    unsigned deleteMin();

    void decreaseKey(unsigned key, unsigned newPriority);

private:

    BufferTreeNode root;

    unsigned blockSize;

    static const DELETE = 0;
    static const DECREASE_KEY = 1;
};