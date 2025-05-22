#include "shannon_decoder.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    std::string inputFile = "output.bin";
    std::string outputFile = "decoded.bin";
    std::string dictFile = "dict.bin";

    if (argc == 4) {
        inputFile = argv[1];
        outputFile = argv[2];
        dictFile = argv[3];
    } else {
        std::cout << "No arguments provided, using default files:\n";
        std::cout << "  Input: " << inputFile << "\n";
        std::cout << "  Output: " << outputFile << "\n";
        std::cout << "  Dict: " << dictFile << "\n";
    }

    try {
        decodeFile(inputFile, outputFile, dictFile);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}