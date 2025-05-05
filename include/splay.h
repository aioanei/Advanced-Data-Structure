#ifndef SPLAY_H
#define SPLAY_H

#include <vector>
#include <numeric>
#include <cassert>
#include <iosfwd>

class SplayTree {
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

    int valueFromKey(int key);
    void print(int l, int r, std::ostream& os); 
    int insert(int value); 
    int smallerEqual(int value); 
    int greaterEqual(int value);
    bool find(int value);
    void erase(int value);
};

#endif
