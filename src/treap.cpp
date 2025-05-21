#include "../include/treap.hpp"
#include <random>
#include <chrono>

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

// static functions and atributes

int Treap::random(int mod) {
    return rng() % mod;
}

const int Treap::INF = 1e9;
const int Treap::INVALID = Treap::INF + 1;

// node functions

Treap::Node::Node(int _val) {
    val = _val;
    sz = 1;
    prio = random(INF);
    left = right = nullptr;
}

Treap::Node::~Node() {
    if (left) delete left;
    if (right) delete right;
}

// treap methods

void Treap::_computeNode(Node* &root) {
    root->sz = 1;
    if (root->left != nullptr)
        root->sz += root->left->sz;
    if (root->right != nullptr)
        root->sz += root->right->sz;
}

int Treap::_getSize(Node* &root) {
    return (root == nullptr) ? 0 : root->sz;
}

Treap::Node* Treap::_join(Node* &left, Node* &right) {
    if (!left || !right) return left ? left : right;

    if (left->prio < right->prio) {
        right->left = _join(left, right->left);
        _computeNode(right);
        return right;
    } else {
        left->right = _join(left->right, right);
        _computeNode(left);
        return left;
    }
}

pair<Treap::Node*, Treap::Node*> Treap::_splitByValue(Node* &node, int val) {
    if (!node) return make_pair(nullptr, nullptr);

    if (node->val <= val) {
        auto [left, right] = _splitByValue(node->right, val);
        node->right = left;
        _computeNode(node);
        return make_pair(node, right);
    } else {
        auto [left, right] = _splitByValue(node->left, val);
        node->left = right;
        _computeNode(node);
        return make_pair(left, node);
    }
}

pair<Treap::Node*, Treap::Node*> Treap::_splitBySize(Node* &node, int size) {
    if (!node) return make_pair(nullptr, nullptr);

    if (_getSize(node->left) + 1 <= size) {
        auto [left, right] = _splitBySize(node->right, size - _getSize(node->left) - 1);
        node->right = left;
        _computeNode(node);
        return make_pair(node, right);
    } else {
        auto [left, right] = _splitBySize(node->left, size);
        node->left = right;
        _computeNode(node);
        return make_pair(left, node);
    }
}

void Treap::_print(Node* &node, int x, int y, ostream &out) {
    if (!node) return;

    if (node->val >= x && node->val <= y) {
        _print(node->left, x, y, out);
        out << node->val << ' ';
        _print(node->right, x, y, out);
    } else if (node->val < x) {
        _print(node->right, x, y, out);
    } else {
        _print(node->left, x, y, out);
    }
}

Treap::Treap() {
    root = nullptr;
}

Treap::~Treap() {
    if (root) delete root;
}

void Treap::insert(int val) {
    pair<Node*, Node*> sol = _splitByValue(root, val - 1);
    pair<Node*, Node*> sol2 = _splitByValue(sol.second, val);

    Node *aux = new Node(val);
    sol2.first = _join(sol2.first, aux);
    sol.second = _join(sol2.first, sol2.second);

    root = _join(sol.first, sol.second);
}

void Treap::erase(int val) {
    pair<Node*, Node*> sol = _splitByValue(root, val - 1);
    pair<Node*, Node*> sol2 = _splitBySize(sol.second, 1);
    root = _join(sol.first, sol2.second);

    delete sol2.first;
}

bool Treap::find(int val) {
    Node *current = root;
    while (current) {
        if (current->val == val) return true;
        if (current->val > val) current = current->left;
        else current = current->right;
    }

    return false;
}

int Treap::greaterOrEqual(int val) {
    Node *current = root;
    int candidate = INVALID;

    while (current) {
        if (current->val >= val) {
            candidate = current->val;
            current = current->left;
        } else
            current = current->right;
    }

    return candidate;
}

int Treap::lessOrEqual(int val) {
    Node *current = root;
    int candidate = INVALID;

    while (current) {
        if (current->val <= val) {
            candidate = current->val;
            current = current->right;
        } else
            current = current->left;
    }

    return candidate;
}

void Treap::print(int x, int y, ostream &out) {
    _print(root, x, y, out);
}
