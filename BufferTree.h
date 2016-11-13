//
// Created by thbl2 on 11/13/2016.
//

#ifndef CS5234_BUFFERTREE_H
#define CS5234_BUFFERTREE_H
#include "BufferTreeNode.h"


class BufferTree {
public:
    BufferTreeNode* root;
    unsigned B;

    BufferTree(unsigned blockSize);

    bool isEmpty();
    void insert(unsigned key, unsigned priority);
    void remove(unsigned key);
    Element deleteMin();

};


#endif //CS5234_BUFFERTREE_H
