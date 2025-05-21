#pragma once

#include "itree.hpp"
#include <chrono>
#include <iostream>
#include <random>

using clk = std::chrono::high_resolution_clock;

enum class OperationType {
    INSERT = 1,
    DELETE = 2,
    FIND = 3,
    LESS_OR_EQUAL = 4,
    GREATER_OR_EQUAL = 5,
    RANGE_QUERY = 6
};

struct Operation {
    OperationType type;
    int value;
    int value2 = 0; // For range queries
};

struct BenchmarkConfig {
    std::string name;
    int num_operations = 5'000'000;
    int value_range = 1'000'000;
    int range_query_max_diff = 100'000;
};

class TestGenerator {
public:
    virtual ~TestGenerator() = default;
    virtual std::string getName() const = 0;
    virtual Operation nextOperation(std::mt19937& gen, const std::vector<int>& inserted) = 0;
};

class Result {
public:
    std::string scenario;
    std::string bst;
    std::size_t n_ops;
    long long ms;

    Result(const std::string& scenario, std::size_t n_ops, long long ms)
        : scenario(scenario), n_ops(n_ops), ms(ms) {}
};

inline void print_csv(const Result& r) {
    std::cout << r.scenario << ','
              << r.bst       << ','
              << r.n_ops    << ','
              << r.ms       << '\n';
}

inline std::mt19937& rng() {
    static thread_local std::mt19937 gen{
        static_cast<std::bernoulli_distribution::result_type>(static_cast<int>(clk::now().time_since_epoch().count()))};
    return gen;
}

template<class Tree>
long long benchmark(Tree& tree, TestGenerator& generator, const BenchmarkConfig& config);