#pragma once
#include <bits/stdc++.h>
#include "../include/itree.h"

using clk = std::chrono::high_resolution_clock;

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
        static_cast<int>(clk::now().time_since_epoch().count())};
    return gen;
}

extern int NUM_OPERATIONS;
extern int VALUE_RANGE;
extern int RANGE_QUERY_MAX_DIFF;


template<class Tree>
long long benchmark(Tree& tree);