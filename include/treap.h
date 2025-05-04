#ifndef TREAP_H
#define TREAP_H

#include <bits/stdc++.h>

using namespace std;

class Treap {
private:
    struct Node {
        int val, prio, sz;
        Node *left, *right;
        Node(int _val = 0);
        ~Node();
    };

    static int random(int mod);
    static const int INF;
    static const int INVALID;

    void _computeNode(Node* &root);
    int _getSize(Node* &root);
    Node* _join(Node* &left, Node* &right);
    pair<Node*, Node*> _splitByValue(Node* &node, int val);
    pair<Node*, Node*> _splitBySize(Node* &node, int size);
    void _print(Node* &node, int x, int y, ostream &out);

public:
    Node* root;
    Treap();
    ~Treap();

    void insert(int val);
    void erase(int val);
    int greaterOrEqual(int val);
    int lessOrEqual(int val);
    void print(int x, int y, ostream &out);
};

#endif