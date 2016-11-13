//
// Created by Nguyen Hung Tam on 11/10/16.
//

#include "PQBTree.h"
#include <iostream>
#include <time.h>
#include <queue>

using namespace std;

map<int, vector<int> > myMap;
map<int, bool> visited;


void addEdge(int x, int y) {
    if (x == y) {
        return;
    } else {
        if (myMap.count(x)) {
            myMap[x].push_back(y);
        } else {
            vector<int> v;
            v.push_back(y);
            myMap[x] = v;
        }

        if (myMap.count(y)) {
            myMap[y].push_back(x);
        } else {
            vector<int> v;
            v.push_back(x);
            myMap[y] = v;
        }
    }
}

void djstra_standard(int x) {
    priority_queue<pii> q;
    q.push(make_pair(0, x));
    visited.clear();
    while (!q.empty()) {
        pair<int, int> p = q.top(); q.pop();
        int priority = p.first;
        int key = p.second;
        if (!visited[key]) {
            visited[key] = true;
            if (!myMap.count(key)) return;
            vector<int> neighbor = myMap[key];
            for(auto i: neighbor) {
                if (!visited[i]) {
                    q.push(make_pair(priority + 1, i));
                }
            }
        }
    }
}

void djstra(int x) {
    PQBTree tree;
    tree.insertNode(0, x);
    visited.clear();
    while (!tree.isEmpty()) {
        pair<int, int> p = tree.extractMin();
        int priority = p.first;
        int key = p.second;
        if (!visited[key]) {
            visited[key] = true;
            if (!myMap.count(key)) return;
            vector<int> neighbor = myMap[key];
            for (auto i: neighbor) {
                if (!visited[i]) {
                    tree.insertNode(priority + 1, i);
                }
            }
        }
    }
}


int main() {
    ios::sync_with_stdio(false);

    int x, y, edgeCount;
    edgeCount = 0;

    freopen("facebook/3980.edges", "r", stdin);
    freopen("3980.txt", "w", stdout);

    while(cin >> x >> y) {
        addEdge(x, y);
        edgeCount++;
    }

    x = (*myMap.begin()).first;


    clock_t c = clock();
    djstra(x);
    c = clock() - c;
    cout << c << endl;

    c = clock();
    djstra_standard(x);
    c = clock() - c;
    cout << c << endl;

    cout << myMap.size() << endl;
    cout << edgeCount << endl;
}