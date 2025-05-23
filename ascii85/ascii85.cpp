#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <cstdint>
#include "ascii85.hpp"

namespace ascii85 {  // <<<<<<<<<<<<<<<<<<<<<<<< BEGIN NAMESPACE

// ENCODE: Convert binary string to ASCII85
std::string encode_ascii85(const std::string& input) {
    std::string output = "<~";
    size_t i = 0;

    while (i < input.size()) {
        uint32_t chunk = 0;
        int len = 0;

        for (int j = 0; j < 4; ++j) {
            chunk <<= 8;
            if (i < input.size()) {
                chunk |= static_cast<unsigned char>(input[i++]);
                ++len;
            }
        }

        if (chunk == 0 && len == 4) {
            output += 'z';
        } else {
            char encoded[5];
            for (int j = 4; j >= 0; --j) {
                encoded[j] = static_cast<char>(chunk % 85 + 33);
                chunk /= 85;
            }
            output.append(encoded, encoded + len + 1);
        }
    }

    output += "~>";
    return output;
}

// DECODE: Convert ASCII85 string to binary string
std::string decode_ascii85_to_string(const std::string& input_raw) {
    if (input_raw.size() < 4 || input_raw.substr(0, 2) != "<~" || input_raw.substr(input_raw.size() - 2) != "~>")
        throw std::runtime_error("Missing <~ or ~> framing");

    std::string input = input_raw.substr(2, input_raw.size() - 4);  // Strip <~ and ~>
    std::vector<char> group;
    std::string output;
    uint32_t value = 0;
    char last_ch = '\0';

    for (char ch : input) {
        if (std::isspace(static_cast<unsigned char>(ch))) continue;

        if (ch == 'z') {
            if (!group.empty())
                throw std::runtime_error("'z' inside group");
            if (last_ch == 'z')
                throw std::runtime_error("Repeated 'z' characters not allowed");
            output.append(4, '\0');
            last_ch = ch;
            continue;
        }

        // STRICT VALIDATION: only allow characters between '!' and 'u'
        if (ch < '!' || ch > 'u')
            throw std::runtime_error("Invalid character in ASCII85");

        group.push_back(ch);
        if (group.size() == 5) {
            value = 0;
            for (char c : group) {
                value = value * 85 + (c - 33);
            }
            for (int i = 3; i >= 0; --i)
                output += static_cast<char>((value >> (i * 8)) & 0xFF);
            group.clear();
        }

        last_ch = ch;
    }

    if (!group.empty()) {
        int padding = 5 - group.size();
        for (int i = 0; i < padding; ++i)
            group.push_back('u');

        value = 0;
        for (char c : group)
            value = value * 85 + (c - 33);

        for (int i = 3; i >= 0; --i)
            if (i >= padding)
                output += static_cast<char>((value >> (i * 8)) & 0xFF);
    }

    return output;
}
} // <<<<<<<<<<<<<<<<<<<<<<<< END NAMESPACE