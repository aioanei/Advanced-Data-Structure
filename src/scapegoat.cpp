#include "../include/scapegoat.hpp"
#include <cmath>

const int ScapegoatTree::INF = 1e9;
const int ScapegoatTree::INVALID = ScapegoatTree::INF + 1;

ScapegoatTree::Node* ScapegoatTree::_insert(int val, Node* node, int &depth) {
    if (!node)
        return new Node(val);

    depth++;
    if (val <= node->value) {
        node->left = _insert(val, node->left, depth);
    } else if (val > node->value) {
        node->right = _insert(val, node->right, depth);
    }

    return node;
}

int ScapegoatTree::_findScapegoat(Node* node, Node* &scapegoat, int value) const {
    if (!node) return 0;

    int leftsize, rightsize;
    if (value < node->value) {
        leftsize = _findScapegoat(node->left, scapegoat, value);
        rightsize = _subtreeSize(node->right);
    } else if (value > node->value) {
        leftsize = _subtreeSize(node->left);
        rightsize = _findScapegoat(node->right, scapegoat, value);
    } else {
        leftsize = _subtreeSize(node->left);
        rightsize = _subtreeSize(node->right);
    }

    int total = leftsize + rightsize + 1;
    if (total > alpha * max_nodes) {
        scapegoat = node;
        return total;
    }

    return total;
}

int ScapegoatTree::_subtreeSize(Node* node) const {
    if (!node) return 0;
    return 1 + _subtreeSize(node->left) + _subtreeSize(node->right);
}

ScapegoatTree::Node* ScapegoatTree::_rebuildTree(Node* node, Node* scapegoat) {
    if (!node) return nullptr;
    if (node == scapegoat) {
        std::vector<Node*> inorder;
        _getInorder(scapegoat, inorder);
        return _buildBalancedTree(inorder);
    }

    node->left = _rebuildTree(node->left, scapegoat);
    node->right = _rebuildTree(node->right, scapegoat);

    return node;
}

void ScapegoatTree::_getInorder(Node* scapegoat, std::vector<Node*>& inorder) {
    if (!scapegoat) return;

    _getInorder(scapegoat->left, inorder);
    inorder.push_back(scapegoat);
    _getInorder(scapegoat->right, inorder);
}

ScapegoatTree::Node* ScapegoatTree::_buildBalancedTree(std::span<Node*> inorder) {
    if (inorder.empty()) return nullptr;

    int mid = inorder.size() / 2;
    Node* root = inorder[mid];
    root->left = _buildBalancedTree(std::span<Node*>(inorder.begin(), inorder.begin() + mid));
    root->right = _buildBalancedTree(std::span<Node*>(inorder.begin() + mid + 1, inorder.end()));

    return root;
}

ScapegoatTree::Node* ScapegoatTree::_erase(Node* node, int val) {
    if (!node) return nullptr;
    if (node->value > val)
        node->left = _erase(node->left, val);
    else if (node->value < val)
        node->right = _erase(node->right, val);
    else {
        if (!node->left) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        if (!node->right) {
            Node* temp = node->left;
            delete node;
            return temp;
        }

        Node* successor = node->right;
        while (successor->left)
            successor = successor->left;

        node->value = successor->value;
        node->right = _erase(node->right, successor->value);
    }
    return node;
}

void ScapegoatTree::_print(Node* &node, int x, int y, std::ostream &out) {
    if (!node) return;

    if (node->value >= x && node->value <= y) {
        _print(node->left, x, y, out);
        out << node->value << ' ';
        _print(node->right, x, y, out);
    } else if (node->value < x) {
        _print(node->right, x, y, out);
    } else {
        _print(node->left, x, y, out);
    }
}

int ScapegoatTree::_maxDepth() const {
    return log2(max_nodes) / log2(1.0 / alpha);
}

ScapegoatTree::ScapegoatTree(double alpha) : alpha(alpha) {
    root = nullptr;
    current_nodes = max_nodes = 0;
}

ScapegoatTree::~ScapegoatTree() {
    std::vector<Node*> inorder;
    _getInorder(root, inorder);
    for (Node* node : inorder)
        delete node;
}

void ScapegoatTree::insert(int val) {
    current_nodes++;
    max_nodes++;

    int depth = 0;
    root = _insert(val, root, depth);

    if (depth > _maxDepth()) {
        Node* scapegoat = nullptr;
        _findScapegoat(root, scapegoat, val);
        root = _rebuildTree(root, scapegoat);
    }
}

void ScapegoatTree::erase(int val) {
    root = _erase(root, val);

    current_nodes--;
    if (current_nodes < alpha * max_nodes) {
        max_nodes = current_nodes;
        root = _rebuildTree(root, nullptr);
    }
}

bool ScapegoatTree::find(int val) {
    Node *current = root;
    while (current) {
        if (current->value == val) return true;
        if (current->value > val) current = current->left;
        else current = current->right;
    }

    return false;
}

int ScapegoatTree::greaterOrEqual(int val) {
    Node *current = root;
    int candidate = INVALID;

    while (current) {
        if (current->value >= val) {
            candidate = current->value;
            current = current->left;
        } else
            current = current->right;
    }

    return candidate;
}

int ScapegoatTree::lessOrEqual(int val) {
    Node *current = root;
    int candidate = INVALID;

    while (current) {
        if (current->value <= val) {
            candidate = current->value;
            current = current->right;
        } else
            current = current->left;
    }

    return candidate;
}

void ScapegoatTree::print(int x, int y, std::ostream& os) {
    _print(root, x, y, os);
    os << '\n';
}
