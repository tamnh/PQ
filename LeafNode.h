//
// Created by thbl2 on 11/10/2016.
//

#ifndef PROJECT_LEAFNODE_H
#define PROJECT_LEAFNODE_H

class LeafNode {

public:

    LeafNode(unsigned lower, unsigned upper);

    void setParent(BufferTreeNode &newParent);
    BufferTreeNode& getParent();

    void insert(unsigned key, unsigned priority);
    void remove(unsigned key);

    std::pair<LeafNode, LeafNode> split();

    std::pair<unsigned, unsigned> getMaxElement();
    std::pair<unsigned, unsigned> getMinElement();

private:

    unsigned lower;
    unsigned upper;

    std::pair<unsigned, unsigned> elements[];
    BufferTreeNode parent;
};


#endif //PROJECT_LEAFNODE_H
