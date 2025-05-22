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

struct SymbolData {
    uint8_t symbol;
    int frequency;
    double probability;
    std::string code;
};

static void giveShannonCodes(std::vector<SymbolData>& symbols, int start, int end) {
    if (start >= end) return;
    if (end - start == 1) return;

    double total = 0;
    for (int i = start; i < end; ++i) total += symbols[i].probability;

    double half = total / 2;
    double sum = 0;
    int mid = start;
    for (; mid < end; ++mid) {
        sum += symbols[mid].probability;
        if (sum >= half) break;
    }

    for (int i = start; i <= mid; ++i) symbols[i].code += '0';
    for (int i = mid + 1; i < end; ++i) symbols[i].code += '1';

    giveShannonCodes(symbols, start, mid + 1);
    giveShannonCodes(symbols, mid + 1, end);
}

std::string symbolToString(uint8_t symbol) {
    if (std::isprint(symbol)) return std::string(1, static_cast<char>(symbol));
    else {
        constexpr char hexDigits[] = "0123456789ABCDEF";
        std::string s = "\\x";
        s += hexDigits[(symbol >> 4) & 0xF];
        s += hexDigits[symbol & 0xF];
        return s;
    }
}

void encodeFile(const std::string& inputFile,
                const std::string& outputFile,
                const std::string& dictFile) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in) {
        std::cerr << "Failed to open input file.\n";
        return;
    }

    std::vector<uint8_t> data((std::istreambuf_iterator<char>(in)), {});
    in.close();

    if (data.empty()) {
        std::cerr << "Input file is empty.\n";
        return;
    }

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

    giveShannonCodes(symbols, 0, symbols.size());

    std::unordered_map<uint8_t, std::string> encodingMap;
    for (const auto& s : symbols) {
        encodingMap[s.symbol] = s.code;
    }

    // e.o
    std::string bitBuffer;
    for (uint8_t byte : data) bitBuffer += encodingMap[byte];

    // Write bbfr
std::ofstream out(outputFile);
if (!out) {
    std::cerr << "Failed to open output file for writing.\n";
    return;
}

out << bitBuffer;  
out.close();
    out.close();

    std::ofstream dictOut(dictFile);
    dictOut << std::left;
    dictOut << std::setw(8) << "Symbol" << " | "
            << std::setw(8) << "Count" << " | "
            << std::setw(12) << "Probability" << " | "
            << "Code\n";

    dictOut << std::string(8, '-') << "-|-" 
            << std::string(8, '-') << "-|-" 
            << std::string(12, '-') << "-|-" 
            << std::string(10, '-') << "\n";

    for (const auto& s : symbols) {
        dictOut << std::setw(8) << symbolToString(s.symbol) << " | "
                << std::setw(8) << s.frequency << " | "
                << std::setw(12) << std::fixed << std::setprecision(6) << s.probability << " | "
                << s.code << "\n";
    }
    dictOut.close();
}