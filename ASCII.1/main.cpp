// MAIN: Command-line interface
#include <iostream>
#include "ascii85.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./ascii85 <encode|decode> <text>" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    std::string data = argv[2];

    if (mode == "encode") {
        std::string encoded = encode_ascii85(data);
        std::cout << encoded << std::endl;
    } else if (mode == "decode") {
        try {
            std::string decoded = decode_ascii85_to_string(data);
            std::cout << decoded << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Decode error: " << e.what() << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Unknown mode: " << mode << std::endl;
        return 1;
    }

    return 0;
}