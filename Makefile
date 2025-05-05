CXX := g++
CXXFLAGS := -std=c++20 -Wall -Iinclude -Ibench -fpermissive -O3 -w

SRCDIR := src
BUILDDIR := build
SOURCES := $(SRCDIR)/main.cpp $(SRCDIR)/splay.cpp $(SRCDIR)/treap.cpp $(SRCDIR)/benchmark.cpp

TARGET := $(BUILDDIR)/main

all: $(TARGET)

$(TARGET): $(SOURCES)
	@mkdir -p $(BUILDDIR)
	@$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	@rm -rf $(BUILDDIR)
	@rm -f results.csv

.PHONY: all clean
