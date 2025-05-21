#include "../include/treap.hpp"
#include "../include/splay.hpp"
#include "../include/scapegoat.hpp"
#include "../include/benchmark.hpp"

#include <fstream>

int main(int argc, char** argv) {
    std::vector<std::string> to_run;
    if (argc == 1) {
        to_run = {"treap", "splay", "scapegoat"};
    } else {
        for (int i = 1; i < argc; ++i)
            to_run.push_back(argv[i]);
    }

    std::ofstream csv("results.csv", std::ios::app);
    if (csv.tellp() == 0)
        csv << "scenario,bst,n_ops,ms\n";

    for (const std::string& bst : to_run) {
        if (bst == "treap") {
            std::cout << "Running treap...\n";
            Treap t;
            long long ms = benchmark(t);
            csv << "mixed_workload,treap," << NUM_OPERATIONS << ',' << ms << '\n';
            std::cout << "Treap  finished in " << ms << " ms\n";
        }
        else if (bst == "splay") {
            SplayTree t;
            long long ms = benchmark(t);
            csv << "mixed_workload,splay," << NUM_OPERATIONS << ',' << ms << '\n';
            std::cout << "Splay finished in " << ms << " ms\n";
        }
        else if (bst == "scapegoat") {
            ScapegoatTree t;
            long long ms = benchmark(t);
            csv << "mixed_workload,scapegoat," << NUM_OPERATIONS << ',' << ms << '\n';
            std::cout << "Scapegoat finished in " << ms << " ms\n";
        }
        else {
            std::cerr << "Unknown tree: " << bst  << '\n';
            return 1;
        }
    }
    return 0;
}
