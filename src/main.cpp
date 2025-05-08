#include "../include/treap.h"
#include "../include/splay.h"
#include "../include/scapegoat.h"
#include "../include/benchmark.h"

int main(int argc, char** argv) {
    NUM_OPERATIONS       = 50'000'000;
    VALUE_RANGE          = 10;
    RANGE_QUERY_MAX_DIFF = 10000;

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
