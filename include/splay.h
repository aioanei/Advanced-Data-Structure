#pragma once

#include <vector>
#include <numeric>
#include <cassert>
#include <iosfwd>
#include "itree.h"

class SplayTree : public ITree {
private:
    struct Node {
        int child[2], parent;
        Node() : parent(0) {
            child[0] = child[1] = 0;
        }
    };
    std::vector<Node> tree;
    std::vector<int> freeindices;
    std::vector<int> v;
    int root;
    void setChild(int x, int y, bool dir);
    int direction(int x);
    void rotate(int x);
    void splay(int x);
    void _insert(int key);
    void _print(int node, int l, int r, std::ostream& os);
    int subtreeMax(int node);

public:
    SplayTree();

    ~SplayTree() override = default;
    int valueFromKey(int key);
    void print(int l, int r, std::ostream& os) override;
    void insert(int value) override;
    int lessOrEqual (int value) override;
    int greaterOrEqual(int value) override;
    bool find(int value);
    void erase(int value) override;
};