#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cstdint>

// Helper: decode hex escape like \x1F to actual byte
uint8_t parseSymbol(const std::string& s) {
    if (s.size() == 1) return static_cast<uint8_t>(s[0]);
    if (s.size() == 4 && s[0] == '\\' && s[1] == 'x') {
        return static_cast<uint8_t>(std::stoi(s.substr(2), nullptr, 16));
    }
    throw std::runtime_error("Invalid symbol format in dictionary: " + s);
}

void decodeFile(const std::string& encodedFile,
                const std::string& dictFile,
                const std::string& outputFile) {
    // Step 1: Parse dictionary
    std::unordered_map<std::string, uint8_t> codeToSymbol;

    std::ifstream dictIn(dictFile);
    if (!dictIn) {
        std::cerr << "Failed to open dictionary file.\n";
        return;
    }

    std::string line;
    bool inTable = false;
    uint8_t paddingBits = 0;

    while (std::getline(dictIn, line)) {
        if (line.find("Symbol") != std::string::npos && line.find("Code") != std::string::npos) {
            inTable = true;
            std::getline(dictIn, line); // skip the dashed line
            continue;
        }

        if (!inTable) continue;
        if (line.find("PaddingBits:") != std::string::npos) {
            std::istringstream iss(line.substr(line.find(":") + 1));
            int pad;
            iss >> pad;
            paddingBits = static_cast<uint8_t>(pad);
            break;
        }

        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string symbolStr, pipe;
        int count;
        double prob;
        std::string code;
        iss >> symbolStr >> pipe >> count >> pipe >> prob >> pipe >> code;

        uint8_t symbol = parseSymbol(symbolStr);
        codeToSymbol[code] = symbol;
    }
    dictIn.close();

    if (codeToSymbol.empty()) {
        std::cerr << "No symbols loaded from dictionary.\n";
        return;
    }

    // Step 2: Read encoded file
    std::ifstream in(encodedFile, std::ios::binary | std::ios::ate);
    if (!in) {
        std::cerr << "Failed to open encoded file.\n";
        return;
    }

    std::streamsize fileSize = in.tellg();
    if (fileSize < 2) {
        std::cerr << "Encoded file is too small.\n";
        return;
    }

    in.seekg(0, std::ios::beg);
    std::vector<char> buffer(fileSize);
    in.read(buffer.data(), fileSize);
    in.close();

    // Step 3: Last byte = number of padding bits
    uint8_t readPaddingBits = static_cast<uint8_t>(buffer.back());
    if (readPaddingBits != paddingBits) {
        //std::cerr << "Warning: mismatch in padding info. Using value from encoded file.\n";
        paddingBits = readPaddingBits;
    }

    // Step 4: Convert bits to a string
    std::string bitString;
    for (size_t i = 0; i < buffer.size() - 1; ++i) {
        uint8_t byte = static_cast<uint8_t>(buffer[i]);
        for (int b = 7; b >= 0; --b) {
            bitString += ((byte >> b) & 1) ? '1' : '0';
        }
    }

    if (paddingBits > 0 && paddingBits <= 7) {
        bitString.erase(bitString.size() - paddingBits);
    }

    // Step 5: Decode using prefix matching
    std::ofstream out(outputFile, std::ios::binary);
    if (!out) {
        std::cerr << "Failed to open output file.\n";
        return;
    }

    std::string currentCode;
    for (char bit : bitString) {
        currentCode += bit;
        if (codeToSymbol.count(currentCode)) {
            out.put(static_cast<char>(codeToSymbol[currentCode]));
            currentCode.clear();
        }
    }

    if (!currentCode.empty()) {
        std::cerr << "Warning: leftover bits after decoding. Possibly malformed data.\n";
    }

    out.close();
    std::cout << "Decoding complete. Output written to: " << outputFile << "\n";
}