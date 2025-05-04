#pragma once
#include <bits/stdc++.h>
#include "../include/itree.h"

using clk = std::chrono::high_resolution_clock;

struct Result {
    std::string scenario;
    std::string bst;
    std::size_t n_ops;
    long long   ms;
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
