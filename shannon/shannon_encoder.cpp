#include "shannon_encoder.hpp"
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <bitset>
#include <iomanip>
#include <iostream>
#include <cstdint>
#include <cctype>
#include <stdexcept>

struct SymbolData {
    uint8_t symbol;
    int frequency;
    double probability;
    std::string code;
};

static void giveshannoncodes(std::vector<SymbolData>& symbols, int start, int end) {
    if (end - start <= 1) return;

    double total = 0.0;
    for (int i = start; i < end; ++i)
        total += symbols[i].probability;

    double bestDiff = total;
    int splitIndex = start;
    double leftSum = 0.0;
    for (int i = start; i < end - 1; ++i) {
        leftSum += symbols[i].probability;
        double diff = std::abs((total - leftSum) - leftSum);
        if (diff < bestDiff) {
            bestDiff = diff;
            splitIndex = i;
        }
    }

    for (int i = start; i <= splitIndex; ++i) symbols[i].code += '0';
    for (int i = splitIndex + 1; i < end; ++i) symbols[i].code += '1';

    giveshannoncodes(symbols, start, splitIndex + 1);
    giveshannoncodes(symbols, splitIndex + 1, end);
}

std::string symbolToString(uint8_t symbol) {
    if (std::isprint(symbol)) return std::string(1, static_cast<char>(symbol));
    char buf[5];
    snprintf(buf, sizeof(buf), "\\x%02X", symbol);
    return std::string(buf);
}

void encodeFile(const std::string& inputFile,
                const std::string& outputFile,
                const std::string& dictFile) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in) throw std::runtime_error("Failed to open input file.");

    std::vector<uint8_t> data((std::istreambuf_iterator<char>(in)), {});
    in.close();
    if (data.empty()) throw std::runtime_error("Input file is empty.");

    std::map<uint8_t, int> freq;
    for (uint8_t byte : data) freq[byte]++;
    int total = static_cast<int>(data.size());

    std::vector<SymbolData> symbols;
    for (auto& [symbol, count] : freq) {
        symbols.push_back({symbol, count, static_cast<double>(count) / total, ""});
    }

    std::sort(symbols.begin(), symbols.end(),
              [](const SymbolData& a, const SymbolData& b) {
                  return a.probability > b.probability;
              });

    giveshannoncodes(symbols, 0, symbols.size());

    std::unordered_map<uint8_t, std::string> encodingMap;
    for (const auto& s : symbols) {
        encodingMap[s.symbol] = s.code;
    }

    // Construct bit string
    std::string bitString;
    for (uint8_t byte : data) bitString += encodingMap[byte];

    std::vector<uint8_t> encodedBytes;
    int bitCount = 0;
    uint8_t byte = 0;
    for (char bit : bitString) {
        byte <<= 1;
        if (bit == '1') byte |= 1;
        ++bitCount;
        if (bitCount == 8) {
            encodedBytes.push_back(byte);
            bitCount = 0;
            byte = 0;
        }
    }
    if (bitCount > 0) {
        byte <<= (8 - bitCount);
        encodedBytes.push_back(byte);
    }

    // Write total bits as first byte
    std::ofstream out(outputFile, std::ios::binary);
    uint8_t totalBits = static_cast<uint8_t>((encodedBytes.size() - 1) * 8 + bitCount);
    out.put(static_cast<char>(totalBits));
    out.write(reinterpret_cast<char*>(encodedBytes.data()), encodedBytes.size());
    out.close();

    //std::cout << "Encoding complete. see dict.bin" \n"

    // Write dictionary
    std::ofstream dictOut(dictFile);
    for (const auto& s : symbols) {
        dictOut << static_cast<int>(s.symbol) << " " << s.code << "\n";
    }
    dictOut.close();
}