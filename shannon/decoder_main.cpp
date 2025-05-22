#include "shannon_decoder.hpp"
#include <string>

int main() {
    std::string compressedFile = "output.bin";
    std::string dictFile = "dict.bin";
    std::string outputFile = "decoded.bin";

    decodeFile(compressedFile, dictFile, outputFile);

    return 0;
}