#include "shannon_decoder.hpp"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdint>

void decodeFile(const std::string& inputFile,
                const std::string& outputFile,
                const std::string& dictFile) {
    std::ifstream dictIn(dictFile);
    if (!dictIn) throw std::runtime_error("Failed to open dictionary file");

    std::unordered_map<std::string, uint8_t> decodeMap;
    std::string line;
    while (std::getline(dictIn, line)) {
        if (line.empty()) continue;
        size_t spacePos = line.find(' ');
        if (spacePos == std::string::npos)
            throw std::runtime_error("Invalid dictionary entry: " + line);
        int symbol = std::stoi(line.substr(0, spacePos));
        std::string code = line.substr(spacePos + 1);
        decodeMap[code] = static_cast<uint8_t>(symbol);
    }
    dictIn.close();

    std::ifstream in(inputFile, std::ios::binary);
    if (!in) throw std::runtime_error("Failed to open encoded file");

    int totalBits = in.get();  // Read bit length
    if (totalBits <= 0) throw std::runtime_error("Invalid bit length header");

    std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(in)), {});
    in.close();

    std::string bits;
    int bitIndex = 0;
    for (uint8_t byte : bytes) {
        for (int i = 7; i >= 0 && bitIndex < totalBits; --i, ++bitIndex) {
            bits += ((byte >> i) & 1) ? '1' : '0';
        }
    }

    std::ofstream out(outputFile, std::ios::binary);
    std::string temp;
    for (char bit : bits) {
        temp += bit;
        if (decodeMap.count(temp)) {
            out.put(static_cast<char>(decodeMap[temp]));
            temp.clear();
        }
    }
    out.close();

    std::cout << "Decoding complete. Output written to: " << outputFile << "\n";
}