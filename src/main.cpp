#include "../include/treap.hpp"
#include "../include/splay.hpp"
#include "../include/scapegoat.hpp"
#include "../include/benchmark.hpp"
#include "../include/test_generators.hpp"

#include <fstream>
#include <map>

int main(int argc, char** argv) {
    std::vector<std::string> tree_types;
    std::vector<std::string> test_types;
    if (argc == 1) {
        tree_types = {"treap", "splay", "scapegoat"};
        test_types = {"mixed_workload"};
    } else {
        bool parsing_tests = false;
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--tests") {
                parsing_tests = true;
                continue;
            }

            if (parsing_tests) {
                test_types.push_back(arg);
            } else {
                tree_types.push_back(arg);
            }
        }

        if (tree_types.empty() || test_types.empty()) {
            std::cerr << "Usage: " << argv[0] << " [tree_types] --tests [test_types]\n";
            std::cerr << "Available tree types: treap, splay, scapegoat\n";
            std::cerr << "Available test types: mixed_workload\n";
            return 1;
        }
    }

    BenchmarkConfig config;

    std::map<std::string, std::unique_ptr<TestGenerator>> generators;
    generators["mixed_workload"] = std::make_unique<MixedWorkloadGenerator>(config);

    std::ofstream csv("results.csv", std::ios::app);
    if (csv.tellp() == 0)
        csv << "scenario,bst,n_ops,ms\n";

    for (const std::string& test_type : test_types) {
        if (generators.find(test_type) == generators.end()) {
            std::cerr << "Unknown test type: " << test_type << '\n';
            continue;
        }

        TestGenerator& generator = *generators[test_type];
        for (const std::string& bst : tree_types) {
            if (bst == "treap") {
                std::cout << "Running treap with " << test_type << "...\n";
                Treap t;
                long long ms = benchmark(t, generator, config);
                csv << "mixed_workload,treap," << config.num_operations << ',' << ms << '\n';
                std::cout << "Treap with " << test_type << " finished in " << ms << " ms\n";
            }
            else if (bst == "splay") {
                std::cout << "Running splay with " << test_type << "...\n";
                SplayTree t;
                long long ms = benchmark(t, generator, config);
                csv << "mixed_workload,splay," << config.num_operations << ',' << ms << '\n';
                std::cout << "Splay with " << test_type << " finished in " << ms << " ms\n";
            }
            else if (bst == "scapegoat") {
                std::cout << "Running scapegoat with " << test_type << "...\n";
                ScapegoatTree t;
                long long ms = benchmark(t, generator, config);
                csv << "mixed_workload,scapegoat," << config.num_operations << ',' << ms << '\n';
                std::cout << "Scapegoat with " << test_type << " finished in " << ms << " ms\n";
            }
            else {
                std::cerr << "Unknown tree: " << bst  << '\n';
                return 1;
            }
        }
    }
    return 0;
}
