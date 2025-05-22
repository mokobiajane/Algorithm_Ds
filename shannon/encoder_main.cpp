#include "shannon_encoder.hpp"

int main() {
    std::string inputFile = "input.bin";
    std::string outputFile = "output.bin";
    std::string dictFile = "dict.bin";

    encodeFile(inputFile, outputFile, dictFile);

    return 0;
}