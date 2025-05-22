CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = encoder

SRCS = encoder_main.cpp shannon_encoder.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) output.bin dict.bin

decoder: decoder_main.o shannon_decoder.o
	$(CXX) $(CXXFLAGS) -o decoder decoder_main.o shannon_decoder.o

clean:
	rm -f $(OBJS) decoder_main.o shannon_decoder.o decoder \
 output.bin dict.bin decoded.bin decoded.bin