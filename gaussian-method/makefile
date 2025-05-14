# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -I /usr/include/eigen3 -Wall -Wextra -O2

# GoogleTest linking
GTEST_LIBS = -lgtest -lgtest_main -pthread

# Targets
TARGET = gauss_calc
TEST_TARGET = guass_test

# Sources
SRCS = main.cpp gauss.cpp
TEST_SRCS = test_gauss.cpp gauss.cpp

# Build the main solver
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build and run tests
test: $(TEST_SRCS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $^ $(GTEST_LIBS)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)
