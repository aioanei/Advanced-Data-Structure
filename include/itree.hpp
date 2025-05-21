#pragma once

// common interface
#include <ostream>

struct ITree {
    virtual ~ITree() = default;
    virtual void insert(int) = 0;
    virtual void erase (int) = 0;
    virtual int greaterOrEqual(int) = 0;
    virtual int lessOrEqual   (int) = 0;
    virtual void print(int x, int y, std::ostream&) = 0;
};
