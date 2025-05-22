#include "shannon_encoder.hpp"
#include "shannon_decoder.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>

bool filesAreEqual(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1, std::ios::binary);
    std::ifstream f2(file2, std::ios::binary);

    std::vector<uint8_t> b1((std::istreambuf_iterator<char>(f1)), {});
    std::vector<uint8_t> b2((std::istreambuf_iterator<char>(f2)), {});
    return b1 == b2;
}

int main() {
    const std::string input = "test_input.bin";
    const std::string encoded = "test_encoded.bin";
    const std::string decoded = "test_decoded.bin";
    const std::string dict = "dict.bin";

    // Write some test binary data
    std::ofstream out(input, std::ios::binary);
    std::string testData = "abacabadabacaba!";
    out.write(testData.data(), testData.size());
    out.close();

    try {
        encodeFile(input, encoded, dict);
        decodeFile(encoded, decoded, dict);
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }

    if (filesAreEqual(input, decoded)) {
        std::cout << "✅ Test passed: decoded file matches original.\n";
        return 0;
    } else {
        std::cerr << "❌ Test failed: decoded output differs from input.\n";
        return 1;
    }
}