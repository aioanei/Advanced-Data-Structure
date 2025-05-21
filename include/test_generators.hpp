#pragma once

#include "benchmark.hpp"

class MixedWorkloadGenerator : public TestGenerator {
private:
    BenchmarkConfig config;

public:
    MixedWorkloadGenerator(const BenchmarkConfig& cfg);

    std::string getName() const override;

    Operation nextOperation(std::mt19937& gen, const std::vector<int>& inserted) override;
};