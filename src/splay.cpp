#include "../include/splay.h"
#include <vector>
#include <numeric>
#include <cassert>
#include <fstream>
#include <iostream> // Include iostream for std::ostream

// Keep original includes and macros for main, if desired
#include <bits/stdc++.h>
#ifdef BLAT
    #include "debug/debug.hpp"
#else
    #define debug(x...)
#endif

using namespace std;

// Global file streams used by main
ifstream in ("abce.in");
ofstream out ("abce.out");

// --- SplayTree Method Implementations ---

void SplayTree::setChild(int x, int y, bool dir) {
    if (x) tree[x].child[dir] = y; // Check if x is valid
    if (y) tree[y].parent = x; // Check if y is valid
}

int SplayTree::direction(int x) {
    int parent = tree[x].parent;

    if (parent == 0) return -1; // No parent
    if (tree[parent].child[0] == x) return 0; // Left child
    if (tree[parent].child[1] == x) return 1; // Right child

    // Should not happen in a standard splay tree unless modified (like LCT)
    // or if x is not actually a child of parent (data corruption)
    assert(false); // Indicate an unexpected state
    return -1;
}

void SplayTree::rotate(int x) {
    int y = tree[x].parent;
    int z = tree[y].parent;
    int dx = direction(x);
    int dy = direction(y); // Can be -1 if y is root

    // Update child pointer of y
    setChild(y, tree[x].child[dx ^ 1], dx);
    // Update child pointer of x
    setChild(x, y, dx ^ 1);
    // Update child pointer of z (if z exists)
    if (dy != -1) {
        setChild(z, x, dy);
    } else {
        // If y was the root, x becomes the new root (parent is 0)
        tree[x].parent = 0;
    }
    // Update parent pointer of x (already done by setChild if z exists)
    // If z did not exist, tree[x].parent is now 0.
}


void SplayTree::splay(int x) {
    if (x == 0) return; // Cannot splay node 0
    while (direction(x) != -1) { // While x is not the root
        int y = tree[x].parent;
        int z = tree[y].parent; // z can be 0
        int dx = direction(x);
        int dy = direction(y); // dy is -1 if y is root

        if (dy != -1) { // If y is not the root (zig-zig or zig-zag)
            if (dx == dy) rotate(y); // zig-zig: rotate parent first
            else rotate(x); // zig-zag: rotate x first
        }
        rotate(x); // Final rotation for zig or second rotation for zig-zig/zag
    }
    root = x; // After splaying, x becomes the root
}


void SplayTree::_insert(int key) {
    if (root == 0) { // Tree is empty
        root = key;
        tree[key].parent = 0; // Ensure new root has no parent
        return;
    }

    int x = root, prevx = 0;
    bool lastDir = 0; // Direction from prevx to x
    while (x != 0) {
        prevx = x;
        if (v[key] < v[x]) { // Compare values, not keys
            x = tree[x].child[0]; // Go left
            lastDir = 0;
        } else {
            // Allow duplicates, go right if equal or greater
            x = tree[x].child[1]; // Go right
            lastDir = 1;
        }
    }

    // Attach the new node 'key' as a child of 'prevx'
    setChild(prevx, key, lastDir);

    // Splay the newly inserted node
    splay(key);
    // root = key; // splay(key) already sets the root
}


void SplayTree::_print(int node, int l, int r, std::ostream& os) {
    if (node == 0) return; // Base case: empty subtree

    // In-order traversal
    _print(tree[node].child[0], l, r, os); // Visit left subtree

    if (l <= v[node] && v[node] <= r) { // Check if current node's value is in range
        os << v[node] << ' ';
    }

    _print(tree[node].child[1], l, r, os); // Visit right subtree
}

int SplayTree::subtreeMax(int node) {
     if (node == 0) return 0; // Handle empty subtree case
    while (tree[node].child[1]) {
        node = tree[node].child[1];
    }
    return node;
}


SplayTree::SplayTree(int n) : tree(1 + n), freeindices(n), v(1 + n), root(0) {
    // Initialize freeindices with keys 1 to n
    std::iota(freeindices.rbegin(), freeindices.rend(), 1); // Fill descending so pop_back gets 1 first
}

int SplayTree::valueFromKey(int key) {
    // Add a check for invalid key 0
    if (key <= 0 || key >= v.size()) {
        // Consider returning a special value or throwing an exception
        // For competitive programming, often a sentinel like -1 or INT_MIN/MAX is used.
        // Returning 0 might be confused with a valid value if 0 is allowed in the tree.
        // Let's return a value unlikely to be in the set, assuming positive integers.
        return -1; // Or handle error appropriately
    }
    return v[key];
}


void SplayTree::print(int l, int r, std::ostream& os) {
    _print(root, l, r, os);
    os << '\n'; // Add newline after printing
}

int SplayTree::insert(int value) {
    assert(!freeindices.empty()); // Ensure there's a free key
    if (freeindices.empty()) {
        // Handle error: tree capacity exceeded
        return 0; // Indicate failure
    }
    int freekey = freeindices.back();
    freeindices.pop_back();
    v[freekey] = value;
    tree[freekey] = Node(); // Reset node state

    _insert(freekey);
    return freekey;
}

// Returns key of the node with the largest value <= value, or 0 if none exists.
int SplayTree::smallerEqual(int value) {
    int x = root, ans = 0;
    while (x != 0) {
        if (v[x] == value) {
            ans = x; // Found exact match
            break; // No need to search further
        }
        if (v[x] < value) {
            ans = x; // Current node is a candidate, try larger values
            x = tree[x].child[1]; // Go right
        } else {
            // v[x] > value
            x = tree[x].child[0]; // Go left
        }
    }

    // If a candidate was found, splay it to the root
    if (ans != 0) {
        splay(ans);
    } else if (root != 0) {
        // If no candidate <= value was found, but the tree is not empty,
        // the closest node might have been the last visited node before hitting null.
        // Splaying the root might be reasonable, or splaying the last non-null node
        // visited during the search if tracked. For simplicity, we can just splay
        // the result 'ans' (which is 0 if nothing found, or the node if found).
        // Splaying 0 does nothing. If ans is non-zero, it's already splayed.
    }

    return ans; // Return the key found, or 0
}

// Returns key of the node with the smallest value >= value, or 0 if none exists.
int SplayTree::greaterEqual(int value) {
    int x = root, ans = 0;
    while (x != 0) {
        if (v[x] == value) {
            ans = x; // Found exact match
            break;
        }
        if (v[x] > value) {
            ans = x; // Current node is a candidate, try smaller values
            x = tree[x].child[0]; // Go left
        } else {
            // v[x] < value
            x = tree[x].child[1]; // Go right
        }
    }

    // Splay the result if found
    if (ans != 0) {
        splay(ans);
    } else if (root != 0) {
       // Similar logic as smallerEqual regarding splaying
    }

    return ans; // Return the key found, or 0
}


bool SplayTree::find(int value) {
    int x = smallerEqual(value); // Splays the node if found or closest smaller
    // Check if the splayed node (now root, if found) actually matches the value
    // Need to handle the case where smallerEqual returns 0
    if (x != 0 && v[x] == value) {
        return true;
    }
    // If smallerEqual didn't find it, try greaterEqual
    x = greaterEqual(value); // Splay if found
    if (x != 0 && v[x] == value) {
        return true;
    }
    return false; // Not found
}


void SplayTree::erase(int value) {
    // Find the node with the exact value. Splay it if found.
    int x = greaterEqual(value); // Find node >= value
    if (x == 0 || v[x] != value) {
         // Node with the exact value doesn't exist, try smallerEqual just in case
         x = smallerEqual(value);
         if (x == 0 || v[x] != value) {
             // Value definitely not in the tree
             return; // Do nothing
         }
    }
    // Now x is the key of the node with the target value, and it's splayed to the root.
    assert(root == x);

    int leftSubtree = tree[x].child[0];
    int rightSubtree = tree[x].child[1];

    // Disconnect children from the node to be deleted
    if (leftSubtree) tree[leftSubtree].parent = 0;
    if (rightSubtree) tree[rightSubtree].parent = 0;

    if (leftSubtree == 0) {
        // No left child, the right subtree becomes the new tree
        root = rightSubtree;
    } else {
        // Find the maximum element in the left subtree
        int newRoot = subtreeMax(leftSubtree);
        // Splay this maximum element to the root of the (now isolated) left subtree
        splay(newRoot); // This splay operates conceptually on the left subtree
                        // After splay, newRoot is the root of the structure formed by leftSubtree
                        // and its parent is 0. Its right child must be 0.
        assert(tree[newRoot].child[1] == 0);

        // Attach the original right subtree as the right child of the new root
        setChild(newRoot, rightSubtree, 1);
        root = newRoot; // Update the main tree root
    }

    // Clean up the deleted node and add its key back to free list
    tree[x] = Node(); // Reset node data
    v[x] = 0; // Reset value (optional, good practice)
    freeindices.push_back(x);
}


// --- Main Function ---

int main() {
    ios_base::sync_with_stdio(false); // Faster I/O
    cin.tie(NULL); // Untie cin/cout

    int q;
    in >> q;
    SplayTree tree(q); // Max q operations, so q nodes suffice

    while (q--) {
        int t, x, y;
        in >> t;
        switch (t) {
            case 1: // Insert
                in >> x;
                tree.insert(x);
                break;

            case 2: // Erase
                in >> x;
                tree.erase(x); // erase now handles non-existent values gracefully
                break;

            case 3: // Find
                in >> x;
                // find already returns bool, no need for valueFromKey
                out << (tree.find(x) ? 1 : 0) << '\n';
                break;

            case 4: // Smaller Equal
                in >> x;
                {
                    int key = tree.smallerEqual(x);
                    if (key != 0) { // Check if a valid key was returned
                        out << tree.valueFromKey(key) << '\n';
                    } else {
                        out << "None\n"; // Or appropriate message/value for not found
                    }
                }
                break;

            case 5: // Greater Equal
                in >> x;
                 {
                    int key = tree.greaterEqual(x);
                    if (key != 0) { // Check if a valid key was returned
                        out << tree.valueFromKey(key) << '\n';
                    } else {
                        out << "None\n"; // Or appropriate message/value for not found
                    }
                }
                break;

            case 6: // Print Range
                in >> x >> y;
                assert(x <= y);
                tree.print(x, y, out); // Pass the output stream
                break;
        }
    }
    in.close();
    out.close();
    return 0;
}
