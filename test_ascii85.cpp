#include "ascii85.hpp"
#include <gtest/gtest.h>

TEST(Ascii85Test, EncodeDecodeRoundTrip) {
    std::string input = "Welcome!";
    EXPECT_EQ(decode_ascii85_to_string(encode_ascii85(input)), input);
}

TEST(Ascii85Test, EmptyString) {
    std::string input = "";
    EXPECT_EQ(encode_ascii85(input), "<~~>");
    EXPECT_EQ(decode_ascii85_to_string("<~~>"), input);
}

TEST(Ascii85Test, NullBytesCompression) {
    std::string input = std::string("\0\0\0\0", 4);
    auto encoded = encode_ascii85(input);
    EXPECT_NE(encoded.find('z'), std::string::npos);
    EXPECT_EQ(decode_ascii85_to_string(encoded), input);
}

TEST(Ascii85Test, InvalidDecode) {
    std::string input = "@@@@~";
    EXPECT_THROW(decode_ascii85_to_string(input), std::runtime_error);
}