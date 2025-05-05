#include "../include/splay.h"
#include <vector>
#include <numeric>
#include <cassert>
#include <iostream>
#include <algorithm> // Add this include for std::max and std::reverse

void SplayTree::setChild(int x, int y, bool dir) {
    if (x) tree[x].child[dir] = y;
    if (y) tree[y].parent = x;
}

int SplayTree::direction(int x) {
    int parent = tree[x].parent;

    if (parent == 0) return -1;
    if (tree[parent].child[0] == x) return 0;
    if (tree[parent].child[1] == x) return 1;

    assert(false);
    return -1;
}

void SplayTree::rotate(int x) {
    int y = tree[x].parent;
    int z = tree[y].parent;
    int dx = direction(x);
    int dy = direction(y);

    setChild(y, tree[x].child[dx ^ 1], dx);
    setChild(x, y, dx ^ 1);
    if (dy != -1) {
        setChild(z, x, dy);
    } else {
        tree[x].parent = 0;
    }

}

void SplayTree::splay(int x) {
    if (x == 0) return;
    while (direction(x) != -1) {
        int y = tree[x].parent;
        int z = tree[y].parent;
        int dx = direction(x);
        int dy = direction(y);
        if (dy != -1) {
            if (dx == dy) rotate(y);
            else rotate(x);
        }
        rotate(x);
    }
    root = x;
}

void SplayTree::_insert(int key) {
    if (root == 0) {
        root = key;
        tree[key].parent = 0;
        return;
    }

    int x = root, prevx = 0;
    bool lastDir = 0;
    while (x != 0) {
        prevx = x;
        if (v[key] < v[x]) {
            x = tree[x].child[0];
            lastDir = 0;
        } else {
            x = tree[x].child[1];
            lastDir = 1;
        }
    }

    setChild(prevx, key, lastDir);

    splay(key);

}

void SplayTree::_print(int node, int l, int r, std::ostream& os) {
    if (node == 0) return;

    _print(tree[node].child[0], l, r, os);

    if (l <= v[node] && v[node] <= r) {
        os << v[node] << ' ';
    }

    _print(tree[node].child[1], l, r, os);
}

int SplayTree::subtreeMax(int node) {
     if (node == 0) return 0;
    while (tree[node].child[1]) {
        node = tree[node].child[1];
    }
    return node;
}

SplayTree::SplayTree() : tree(1), v(1), root(0) {
    // Initialize with only the sentinel node 0
    // freeindices is initially empty
}

int SplayTree::valueFromKey(int key) {
    if (key <= 0 || key >= v.size()) {
        return -1;
    }
    return v[key];
}

void SplayTree::print(int l, int r, std::ostream& os) {
    _print(root, l, r, os);
    os << '\n';
}

void SplayTree::insert(int value) {
    if (freeindices.empty()) {
        size_t old_size = tree.size();
        size_t new_size = std::max(old_size + 1, old_size * 2);
        tree.resize(new_size);
        v.resize(new_size);
        for (size_t i = old_size; i < new_size; ++i) {
            freeindices.push_back(i);
        }
        std::reverse(freeindices.begin(), freeindices.end());
    }

    assert(!freeindices.empty());
    int freekey = freeindices.back();
    freeindices.pop_back();
    v[freekey] = value;
    tree[freekey] = Node();
    _insert(freekey);
}

int SplayTree::lessOrEqual(int value) {
    int x = root, ans = 0;
    while (x != 0) {
        if (v[x] == value) {
            ans = x;
            break;
        }
        if (v[x] < value) {
            ans = x;
            x = tree[x].child[1];
        } else {
            x = tree[x].child[0];
        }
    }

    if (ans != 0) {
        splay(ans);
    }

    return ans;
}

int SplayTree::greaterOrEqual(int value) {
    int x = root, ans = 0;
    while (x != 0) {
        if (v[x] == value) {
            ans = x;
            break;
        }
        if (v[x] > value) {
            ans = x;
            x = tree[x].child[0];
        } else {
            x = tree[x].child[1];
        }
    }

    if (ans != 0) {
        splay(ans);
    }

    return ans;
}

bool SplayTree::find(int value) {
    int x = lessOrEqual(value);
    if (x != 0 && v[x] == value) {
        return true;
    }
    x = greaterOrEqual(value);
    if (x != 0 && v[x] == value) {
        return true;
    }
    return false;
}

void SplayTree::erase(int value) {
    int x = greaterOrEqual(value);
    if (x == 0 || v[x] != value) {
         x = lessOrEqual(value);
         if (x == 0 || v[x] != value) {
             return;
         }
    }
    assert(root == x);

    int leftSubtree = tree[x].child[0];
    int rightSubtree = tree[x].child[1];

    if (leftSubtree) tree[leftSubtree].parent = 0;
    if (rightSubtree) tree[rightSubtree].parent = 0;

    if (leftSubtree == 0) {
        root = rightSubtree;
    } else {
        int newRoot = subtreeMax(leftSubtree);
        splay(newRoot);
        assert(tree[newRoot].child[1] == 0);
        setChild(newRoot, rightSubtree, 1);
        root = newRoot;
    }

    tree[x] = Node();
    v[x] = 0;
    freeindices.push_back(x);
}
