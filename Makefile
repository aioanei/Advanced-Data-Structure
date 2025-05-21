CXX := g++
CXXFLAGS := -std=c++20 -Wall -Iinclude -Ibench -fpermissive -fsanitize=address,undefined,signed-integer-overflow -O3 -w

SRCDIR := src
BUILDDIR := build
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

TARGET := $(BUILDDIR)/main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	@rm -rf $(BUILDDIR)
	@rm -f results.csv

-include $(DEPS)

.PHONY: all clean
