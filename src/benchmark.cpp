#include "../include/treap.hpp"
#include "../include/splay.hpp"
#include "../include/scapegoat.hpp"
#include "../include/benchmark.hpp"
#include "../include/test_generators.hpp"

template<class Tree>
long long benchmark(Tree& tree, TestGenerator& generator, const BenchmarkConfig& config) {
    std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    std::vector<int> inserted;
    inserted.reserve(config.num_operations);

    auto t0 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < config.num_operations; ++i) {
        if (i % 1'000'000 == 0)
            std::cerr << "Operation " << i + 1 << " / " << config.num_operations << "\r";

        auto op = generator.nextOperation(rng, inserted);
        switch (op.type) {
            case OperationType::INSERT:
                tree.insert(op.value);
                inserted.push_back(op.value);
                break;
            case OperationType::DELETE:
                tree.erase(op.value);
                break;
            case OperationType::FIND:
                tree.find(op.value);
                break;
            case OperationType::LESS_OR_EQUAL:
                tree.lessOrEqual(op.value);
                break;
            case OperationType::GREATER_OR_EQUAL:
                tree.greaterOrEqual(op.value);
                break;
            case OperationType::RANGE_QUERY:
                tree.lessOrEqual(op.value2);
                break;
        }
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    return duration;
}

template long long benchmark<Treap>(Treap&, TestGenerator&, const BenchmarkConfig&);
template long long benchmark<SplayTree>(SplayTree&, TestGenerator&, const BenchmarkConfig&);
template long long benchmark<ScapegoatTree>(ScapegoatTree&, TestGenerator&, const BenchmarkConfig&);