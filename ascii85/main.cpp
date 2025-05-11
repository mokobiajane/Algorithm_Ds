// MAIN: Command-line interface
#include <iostream>
#include "ascii85.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./ascii85 <-e|-d> <text>" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    std::string data = argv[2];

    if (mode == "-e") {
        std::string encoded = ascii85::encode_ascii85(data);
        std::cout << encoded << std::endl;
    } else if (mode == "-d") {
        try {
            std::string decoded =ascii85::decode_ascii85_to_string(data);
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