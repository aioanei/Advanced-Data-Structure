#pragma once

#include "itree.h"

class ScapegoatTree : public ITree {
private:
    static const int INF;
    static const int INVALID;

    struct Node {
        Node *left, *right;
        int value;

        Node() {
            left = right = nullptr;
            value = 0;
        }

        Node(int val) {
            left = right = nullptr;
            value = val;
        }
    };

    Node *root;
    int current_nodes;
    int max_nodes;
    double alpha;

    Node* _insert(int val, Node* node, int &depth);
    int _findScapegoat(Node* node, Node* &scapegoat, int value) const;
    int _subtreeSize(Node* node) const;
    Node* _rebuildTree(Node* node, Node* scapegoat);
    void _getInorder(Node* scapegoat, std::vector<Node*> &inorder);
    Node* _buildBalancedTree(std::span<Node*> inorder);
    Node* _erase(Node* node, int val);
    void _print(Node* &node, int x, int y, std::ostream &out);
    int _maxDepth() const;

public:
    ScapegoatTree(double alpha = 0.75);
    ~ScapegoatTree() override;

    void print(int l, int r, std::ostream& os) override;
    void insert(int value) override;
    int lessOrEqual(int value) override;
    int greaterOrEqual(int value) override;
    bool find(int value);
    void erase(int value) override;
};