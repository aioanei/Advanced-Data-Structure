CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude -Ibench -fpermissive

SRCDIR := src
SOURCES := $(SRCDIR)/benchmark_splay.cpp $(SRCDIR)/splay.cpp $(SRCDIR)/treap.cpp

TARGET := $(SRCDIR)/benchmark_splay.exe

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean
