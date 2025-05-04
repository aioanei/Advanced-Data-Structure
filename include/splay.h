#ifndef SPLAY_H
#define SPLAY_H

#include <vector>
#include <numeric> // For std::iota
#include <cassert>
#include <iosfwd>  // For std::ostream forward declaration

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
    std::vector<int> v; // Stores the actual values associated with keys
    int root;

    // Private helper methods
    void setChild(int x, int y, bool dir);
    int direction(int x);
    void rotate(int x);
    void splay(int x);
    void _insert(int key);
    void _print(int node, int l, int r, std::ostream& os);
    int subtreeMax(int node); // Renamed parameter for clarity

public:
    // Constructor
    SplayTree(int n);

    // Public interface
    int valueFromKey(int key); // Get value associated with a tree key
    void print(int l, int r, std::ostream& os); // Print values in range [l, r]
    int insert(int value); // Insert value, returns the key used
    int smallerEqual(int value); // Key of largest value <= given value, 0 if none
    int greaterEqual(int value); // Key of smallest value >= given value, 0 if none
    bool find(int value); // Check if value exists
    void erase(int value); // Erase value if present
};

#endif // SPLAY_H
