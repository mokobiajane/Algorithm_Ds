CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -I. -I/usr/include/eigen3 -I/usr/src/gtest/include

TARGET = gauss_calc
TEST = test_gauss

SRC = main.cpp gauss.cpp
TEST_SRC = test_gauss.cpp gauss.cpp

.PHONY: all clean test

all: $(TARGET)


$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -o $(TEST) $^ -lgtest -lgtest_main -pthread
	./$(TEST)

clean:
	rm -f $(TARGET) $(TEST)
