To build Task 1.1(Decoder):

run:g++ -std=c++11 -c ascii85.cpp
g++ -std=c++11 -c main.cpp
g++ ascii85.o main.o -o ascii85

./ascii85 encode <input>
./ascii85 decode <"encode result">