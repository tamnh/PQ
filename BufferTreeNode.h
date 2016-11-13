//
// Created by thbl2 on 11/11/2016.
//
#include <map>
#include <vector>

#ifndef CS5234_NODE_H
#define CS5234_NODE_H

typedef std::pair<unsigned, unsigned> Element;

struct keyCompare {
    bool operator() (Element e1, Element e2) {
        return (e1.first < e2.first);
    }
};

struct prioCompare {
    bool operator() (Element e1, Element e2) {
        return (e1.second < e2.second);
    }
};

class BufferTreeNode {

public:

    std::vector<Element> elements;
    std::vector<BufferTreeNode*> children;

    std::vector<Element> smallElements;
    std::map<unsigned, int> buffer;

    bool leaf;

    keyCompare keyComp;
    prioCompare prioComp;

    unsigned maxNoElements;
    unsigned minNoElements;

    unsigned maxOp;

    unsigned maxSmall;
    unsigned minSmall;

    BufferTreeNode(unsigned minE, unsigned maxE, unsigned maxS, unsigned minS, unsigned maxO);

    Element getMax();
    Element getMin();

    unsigned getNoElements();

    BufferTreeNode* getSameTypeNode();

    void split(unsigned idx);
    unsigned merge(unsigned idx);

    void add(unsigned key, unsigned priority);
    void remove(unsigned key);
    Element deleteMin();

    void refillSmallElements();
    void propagateOp();

    bool isLeaf();
    bool isFull();
    bool isLow();
    bool isOpFull();
    bool isSmallLow();
};


#endif //CS5234_NODE_H
