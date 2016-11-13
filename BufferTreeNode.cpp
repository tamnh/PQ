//
// Created by thbl2 on 11/11/2016.
//

#include "BufferTreeNode.h"
#include <algorithm>

using std::vector;
using std::map;
using std::pair;

BufferTreeNode::BufferTreeNode(unsigned minE, unsigned maxE, unsigned maxS, unsigned minS, unsigned maxO)
{
    minNoElements = minE;
    maxNoElements = maxE;
    minSmall = minS;
    maxSmall = maxS;
    maxOp = maxO;

    elements = vector<Element>(maxE);
    children = vector<BufferTreeNode*>(maxE);
    smallElements = vector<Element>(maxS);
    buffer = map<unsigned, int>();
    leaf = true;
}

bool BufferTreeNode::isLeaf() {
    return leaf;
}

bool BufferTreeNode::isFull() {
    return (elements.size() == maxNoElements);
}

bool BufferTreeNode::isLow() {
    return (elements.size() < minNoElements);
}

bool BufferTreeNode::isOpFull() {
    return (buffer.size() == maxOp);
}

bool BufferTreeNode::isSmallLow() {
    return (smallElements.size() < minSmall);
}

Element BufferTreeNode::getMax() {
    return elements.back();
}

Element BufferTreeNode::getMin() {
    return elements.front();
}

unsigned BufferTreeNode::getNoElements() {
    return (unsigned) elements.size();
}

BufferTreeNode* BufferTreeNode::getSameTypeNode() {
    return new BufferTreeNode(minNoElements, maxNoElements, minSmall, maxSmall, maxOp);
}

void BufferTreeNode::split(unsigned idx)
{
    BufferTreeNode* child = children.at(idx);
    BufferTreeNode* child1 = child->getSameTypeNode();
    BufferTreeNode* child2 = child->getSameTypeNode();

    // split elements and children
    unsigned n = child->getNoElements();
    for (unsigned i=0; i<n; i++) {
        if (i<n/2) {
            child1->elements.push_back(child->elements.at(i));
            child1->children.push_back(child->children.at(i));
        }
        else {
            child2->elements.push_back(child->elements.at(i));
            child2->children.push_back(child->children.at(i));
        }
    }

    //split operation buffer
    Element pivot = child1->getMax();
    for (auto kv: buffer) {
        if (kv.first <= pivot.first) {
            child1->buffer.insert(kv);
        }
        else {
            child2->buffer.insert(kv);
        }
    }

    // split small elements store
    std::sort(smallElements.begin(), smallElements.end(), keyComp);
    vector<Element>::iterator it = smallElements.begin();
    while (it != smallElements.end()) {
        if (it->first <= pivot.first) {
            child1->smallElements.push_back(*it);
        }
        else {
            child2->smallElements.push_back(*it);
        }
        ++it;
    }
    if (child1->isSmallLow()) {
        child1->refillSmallElements();
    }
    if (child2->isSmallLow()) {
        child2->refillSmallElements();
    }

    vector<Element>::iterator it1 = elements.begin() + idx;
    vector<BufferTreeNode*>::iterator it2 = children.begin() + idx;
    it1 = elements.erase(it1);
    elements.insert(it1, child2->getMax());
    elements.insert(it, child1->getMax());
    it2 = children.erase(it2);
    children.insert(it2, child2);
    children.insert(it2, child1);
}

unsigned BufferTreeNode::merge(unsigned idx)
{
    unsigned i1 = (idx == 0)? 0:idx-1;
    unsigned i2 = (idx == 0)? 1:idx;
    BufferTreeNode* child1 = children.at(i1);
    BufferTreeNode* child2 = children.at(i2);
    unsigned n1 = child1->getNoElements();
    unsigned n2 = child2->getNoElements();
    unsigned n = n1+n2;
    if (n > maxNoElements) {
        //Balance
        BufferTreeNode* newChild1 = child1->getSameTypeNode();
        BufferTreeNode* newChild2 = child2->getSameTypeNode();
        unsigned h = n/2;
        vector<Element>::iterator eit = child1->elements.begin();
        vector<BufferTreeNode*>::iterator cit = child1->children.begin();
        unsigned i = 0;
        while (eit != child2->elements.end()) {
            if (eit == child1->elements.end()) {
                eit = child2->elements.begin();
                cit = child2->children.begin();
            }
            if (i < h) {
                newChild1->elements.push_back(*eit);
                newChild1->children.push_back(*cit);
            } else {
                newChild2->elements.push_back(*eit);
                newChild2->children.push_back(*cit);
            }
            ++i;
            ++eit;
            ++cit;
        }
        // balance buffer
        Element pivot = newChild1->getMax();
        map<unsigned, int>::iterator oit = child1->buffer.begin();
        while (oit != child2->buffer.end()) {
            if (oit == child1->buffer.end()) oit = child2->buffer.begin();
            if ((*oit).first <= pivot.first) {
                newChild1->buffer.insert(*oit);
            }
            else {
                newChild2->buffer.insert(*oit);
            }
        }

        // balance small store
        std::sort(child1->smallElements.begin(), child1->smallElements.end(), keyComp);
        std::sort(child2->smallElements.begin(), child2->smallElements.end(), keyComp);
        vector<Element>::iterator sit = child1->smallElements.begin();
        while (sit != child2->smallElements.end()) {
            if (sit == child1->smallElements.end()) sit = child2->smallElements.begin();
            if ((*sit).first <= pivot.first) {
                newChild1->smallElements.push_back(*sit);
            }
            else {
                newChild2->smallElements.push_back(*sit);
            }
        }
        if (newChild1->isSmallLow()) newChild1->refillSmallElements();
        if (newChild2->isSmallLow()) newChild2->refillSmallElements();

        elements[i1] = newChild1->getMax();
        children[i1] = newChild1;
        elements[i2] = newChild2->getMax();
        children[i2] = newChild2;
        return i2;
    }
    else {
        //Merge
        BufferTreeNode* newChild = child1->getSameTypeNode();
        for (unsigned i=0; i<n ; ++i) {
            if (i<n1) {
                newChild->elements.push_back(child1->elements[i]);
                newChild->children.push_back(child1->children[i]);
            }
            else {
                newChild->elements.push_back(child2->elements[i]);
                newChild->children.push_back(child2->children[i]);
            }
        }
        //Merge buffer
        map<unsigned, int>::iterator oit = child1->buffer.begin();
        while (oit != child2->buffer.end()) {
            if (oit == child1->buffer.end()) oit = child2->buffer.begin();
            newChild->buffer.insert(*oit);
        }

        //Merge small store
        vector<Element>::iterator sit = child1->smallElements.begin();
        while (sit != child2->smallElements.end()) {
            if (sit == child1->smallElements.end()) sit = child2->smallElements.begin();
            newChild->smallElements.push_back(*sit);
        }
        std::sort(newChild->smallElements.begin(), newChild->smallElements.end(), prioComp);
        newChild->smallElements.erase(newChild->smallElements.begin() + newChild->maxSmall, newChild->smallElements.end());

        elements[i1] = newChild->getMax();
        children[i2] = newChild;
        elements.erase(elements.begin() + i2);
        return i1;
    }
}

void BufferTreeNode::add(unsigned key, unsigned priority) {
    bool found = false;
    for (unsigned j=0; j<smallElements.size(); ++j) {
        auto &e = smallElements[j];
        if (e.first == key) {
            found = true;
            if (e.second > priority) {
                e.second = priority;
                for (unsigned i=j; i>0; --i) {
                    if (smallElements[i].second > smallElements[i-1].second) break;
                    auto temp = smallElements[i];
                    smallElements[i] = smallElements[i-1];
                    smallElements[i-1] = temp;
                }
            }
            break;
        }
    }
    if (!found) {
        unsigned i = (unsigned) smallElements.size();
        while (i > 0) {
            if (smallElements[i].second > priority) {
                smallElements[i] = smallElements[i - 1];
                --i;
            } else break;
        }
        smallElements[i] = std::make_pair(key, priority);
    }
    if (isLeaf()) {
        unsigned low = 0;
        unsigned high = getNoElements()-1;
        while (high >= low) {
            unsigned mid = (high + low)/2;
            if (elements[mid].first == key) {
                low = mid;
                break;
            }
            else if (elements[mid].first < key) {
                low = mid+1;
            }
            else {
                high = mid-1;
            }
        }
        if (elements[low].first == key) {
            if (elements[low].second > priority) elements[low].second = priority;
        }
        else {
            elements.insert(elements.begin() + low, std::make_pair(key, priority));
            children.push_back(NULL);
        }
    }
    else {
        if (buffer.count(key) == 1) {
            if (buffer[key] > priority) {
                buffer[key] = priority;
            }
            else if (buffer[key] == -1) {
                buffer.erase(key);
            }
        }
        else {
            buffer.insert(key, priority);
        }
        if (isOpFull()) {
            propagateOp();
        }
    }
}

void BufferTreeNode::remove(unsigned key) {
    vector<Element>::iterator it = smallElements.begin();
    while (it != smallElements.end()) {
        if (it->first == key) {
            smallElements.erase(it);
            break;
        }
    }
    if (isLeaf()) {
        unsigned low = 0;
        unsigned high = getNoElements()-1;
        while (high >= low) {
            unsigned mid = (high + low)/2;
            if (elements[mid].first == key) {
                low = mid;
                break;
            }
            else if (elements[mid].first < key) {
                low = mid+1;
            }
            else {
                high = mid-1;
            }
        }
        if (elements[low].first == key) {
            elements.erase(elements.begin() + low);
        }
    }
    else {
        if (buffer.count(key) == 1) {
            if (buffer[key] > 0) buffer.erase(key);
        }
        else {
            buffer[key] = -1;
            if (isOpFull()) propagateOp();
        }
    }
}

void BufferTreeNode::propagateOp() {
    unsigned n = 0;
    map<unsigned, int>::iterator it = buffer.begin();
    while (it != buffer.end()) {
        if (n == elements.size() || it->first <= elements[n].first) {
            if (it->second == -1) {
                children[n]->remove(it->first);
            }
            else {
                children[n]->add(it->first, (unsigned) it->second);
            }
            it = buffer.erase(it);
        }
        else {
            ++n;
        }
    }
    n = 0;
    while (n < children.size()) {
        if (children[n]->isFull()) {
            split(n);
            n+=2;
        }
        else if (children[n]->isLow()) {
            n = merge(n)+1;
        }
    }
}

void BufferTreeNode::refillSmallElements() {
    if (isLeaf()) {
        unsigned n = 0;
        smallElements.clear();
        for (n=0; n < elements.size(); ++n) {
            smallElements.push_back(elements[n]);
        }
        std::sort(smallElements.begin(), smallElements.end(), prioComp);
        vector<Element>::iterator it = smallElements.begin() + maxSmall + 1;
        while (it != smallElements.end()) {
            it = smallElements.erase(it);
        }
    }
    else {
        propagateOp();
        smallElements.clear();
        for (auto child: children) {
            for (Element e: child->smallElements) {
                smallElements.push_back(e);
            }
        }
        std::sort(smallElements.begin(), smallElements.end(), prioComp);
        vector<Element>::iterator it = smallElements.begin() + maxSmall + 1;
        while (it != smallElements.end()) {
            it = smallElements.erase(it);
        }
    }
}

Element BufferTreeNode::deleteMin() {
    Element res = smallElements.front();
    smallElements.erase(smallElements.begin());
    if (isSmallLow()) {
        refillSmallElements();
    }
    return res;
}



