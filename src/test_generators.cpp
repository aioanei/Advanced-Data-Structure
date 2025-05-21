#include "../include/test_generators.hpp"

MixedWorkloadGenerator::MixedWorkloadGenerator(const BenchmarkConfig& cfg) : config(cfg) {}

std::string MixedWorkloadGenerator::getName() const {
    return "Mixed Workload";
}

Operation MixedWorkloadGenerator::nextOperation(std::mt19937& gen, const std::vector<int>& inserted) {
    auto t0 = std::chrono::high_resolution_clock::now();
    std::uniform_int_distribution<int> op_dist(1, 6);
    std::uniform_int_distribution<int> val_dist(0, config.value_range - 1);
    std::uniform_int_distribution<int> diff_dist(0, config.range_query_max_diff - 1);

    int op = op_dist(gen);
    int value = val_dist(gen);

    if ((op >= 2 && op <= 5) && inserted.empty())
        op = 1;

    if (op == 2 && !inserted.empty()) {
        std::uniform_int_distribution<int> pick(0, inserted.size() - 1);
        value = inserted[pick(gen)];
    }

    OperationType operation_type;
    switch (op) {
        case 1: operation_type = OperationType::INSERT; break;
        case 2: operation_type = OperationType::DELETE; break;
        case 3: operation_type = OperationType::FIND; break;
        case 4: operation_type = OperationType::LESS_OR_EQUAL; break;
        case 5: operation_type = OperationType::GREATER_OR_EQUAL; break;
        case 6: operation_type = OperationType::RANGE_QUERY; break;
        default: operation_type = OperationType::INSERT; break;
    }

    Operation operation;
    operation.type = operation_type;
    operation.value = value;

    if (operation.type == OperationType::RANGE_QUERY) {
        int v2 = val_dist(gen);
        int l  = std::min(value, v2);
        int r  = std::max(value, v2);
        if (r - l > config.range_query_max_diff) {
            r = l + diff_dist(gen);
            if (r >= config.value_range) r = config.value_range - 1;
            if (l > r) l = r;
        }
        operation.value2 = r;
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    return operation;
}