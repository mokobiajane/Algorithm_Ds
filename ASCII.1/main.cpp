#include <iostream>
#include <string>

// Encoding function
std::string encode(const std::string& input) {
    std::string output;
    for (char c : input) {
        output += (c + 1); // Simple encoding: shift each character by 1
    }
    return output;
}

// Decoding function
std::string decode(const std::string& input) {
    std::string output;
    for (char c : input) {
        output += (c - 1); // Simple decoding: reverse the shift
    }
    return output;
}

// Input validation for decoding
bool isValidInput(const std::string& input) {
    for (char c : input) {
        if (!isalnum(c)) { // Check if input contains only alphanumeric characters
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    // Validate the number of arguments
    if (argc != 3) {
        std::cerr << "Usage: ./utility <mode> <input>\n";
        return 1; // Non-zero exit code for error
    }

    std::string mode = argv[1];
    std::string input = argv[2];

    // Check for valid mode
    if (mode == "encoder") {
        std::cout << encode(input) << '\n';
    } else if (mode == "decoder") {
        if (!isValidInput(input)) {
            std::cerr << "Invalid input for decoding.\n";
            return 1;
        }
        std::cout << decode(input) << '\n';
    } else {
        std::cerr << "Invalid mode. Use 'encoder' or 'decoder'.\n";
        return 1;
    }

    return 0; // Exit successfully
}