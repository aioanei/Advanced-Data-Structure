#include "../include/splay.h"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <numeric>
#include <algorithm> 
#include <set> 
#include <ostream> 
#include <streambuf> 
const int NUM_OPERATIONS = 500000; 
const int MAX_NODES = NUM_OPERATIONS; 
const int VALUE_RANGE = 1000000; 
const int RANGE_QUERY_MAX_DIFF = 10000; 
int main() {
    std::cout << "Starting Splay Tree Benchmark..." << std::endl;
    std::cout << "Number of operations: " << NUM_OPERATIONS << std::endl;
    std::cout << "Value range: [0, " << VALUE_RANGE << ")" << std::endl;

    SplayTree tree(MAX_NODES);
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count()); 
    std::uniform_int_distribution<int> op_dist(1, 6); 
    std::uniform_int_distribution<int> val_dist(0, VALUE_RANGE - 1); 

    std::ostream null_stream(nullptr);

    std::vector<int> inserted_values; 
    inserted_values.reserve(NUM_OPERATIONS);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        int operation = op_dist(rng);
        int value = val_dist(rng);
        if ((operation == 2 || operation == 3 || operation == 4 || operation == 5) && inserted_values.empty()) {
            operation = 1; 
        }
        if (operation == 2 && !inserted_values.empty()) {
             std::uniform_int_distribution<int> existing_val_dist(0, inserted_values.size() - 1);
             value = inserted_values[existing_val_dist(rng)];
        }

        switch (operation) {
            case 1: 
                {
                    int key = tree.insert(value);
                    if (key != 0) { 
                        inserted_values.push_back(value);
                    }
                }
                break;
            case 2:
                tree.erase(value);
                break;
            case 3:
                tree.find(value);
                break;
            case 4:
                tree.smallerEqual(value);
                break;
            case 5: 
                tree.greaterEqual(value);
                break;
            case 6:
                {
                    int val2 = val_dist(rng);
                    int l = std::min(value, val2);
                    int r = std::max(value, val2);
                    if (r - l > RANGE_QUERY_MAX_DIFF) {
                         r = l + rng() % RANGE_QUERY_MAX_DIFF; 
                         if (r >= VALUE_RANGE) r = VALUE_RANGE -1;
                         if (l > r) l = r; 
                    }
                    
                    tree.print(l, r, null_stream);
                }
                break;
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "Benchmark finished." << std::endl;
    std::cout << "Total time elapsed: " << duration.count()/1000 << "s" << std::endl;

    return 0;
}
