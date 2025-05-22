CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
OBJS = shannon_encoder.cpp shannon_decoder.cpp

# Targets
all: encoder decoder

encoder: encoder_main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o encoder encoder_main.cpp $(OBJS)

decoder: decoder_main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o decoder decoder_main.cpp $(OBJS)

# Run full encode-decode test
test: shannon_test
	./shannon_test

shannon_test: shannon_test.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o shannon_test shannon_test.cpp $(OBJS)
	chmod +x shannon_test

clean:
	rm -f encoder decoder shannon_test *.bin *.txt
	