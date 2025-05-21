#include "../include/treap.hpp"
#include "../include/splay.hpp"
#include "../include/scapegoat.hpp"
#include "../include/benchmark.hpp"

const int NUM_OPERATIONS       = 5'000'000;
const int VALUE_RANGE          = 1'000'000;
const int RANGE_QUERY_MAX_DIFF = 10000;

template<class Tree>
long long benchmark(Tree& tree) {
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> op_dist (1, 6);
    std::uniform_int_distribution<int> val_dist(0, VALUE_RANGE - 1);
    std::uniform_int_distribution<int> diff_dist(0, RANGE_QUERY_MAX_DIFF - 1);

    std::vector<int> inserted;
    inserted.reserve(NUM_OPERATIONS);

    auto t0 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        if (i % 1'000'000 == 0)
            std::cerr << "Operation " << i << " of " << NUM_OPERATIONS << '\n';
        // std::cerr << i << '\n';
        int op    = op_dist(rng);
        int value = val_dist(rng);

        if ((op >= 2 && op <= 5) && inserted.empty())
            op = 1;

        if (op == 2 && !inserted.empty()) {
            std::uniform_int_distribution<int> pick(0, inserted.size()-1);
            value = inserted[pick(rng)];
        }

        switch (op) {
        case 1:
            tree.insert(value);
            inserted.push_back(value);
            break;
        case 2:
            tree.erase(value);
            std::erase(inserted, value);
            break;
        case 3:
            tree.greaterOrEqual(value);
            break;
        case 4:
            tree.lessOrEqual(value);
            break;
        case 5:
            tree.greaterOrEqual(value);
            break;
        case 6: {
            int v2 = val_dist(rng);
            int l  = std::min(value, v2);
            int r  = std::max(value, v2);
            if (r - l > RANGE_QUERY_MAX_DIFF) {
                r = l + diff_dist(rng);
                if (r >= VALUE_RANGE) r = VALUE_RANGE - 1;
                if (l > r) l = r;
            }
            std::ostream null_out(nullptr);
            tree.print(l, r, null_out);
            break; }
        }
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
}

template long long benchmark<Treap>(Treap&);
template long long benchmark<SplayTree>(SplayTree&);
template long long benchmark<ScapegoatTree>(ScapegoatTree&);
