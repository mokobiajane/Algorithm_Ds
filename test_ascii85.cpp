#include "ascii85.hpp"
#include <gtest/gtest.h>
#include <string>

std::string encode(const std::string& input) {
    return ascii85::encode_ascii85(input);
}

std::string decode(const std::string& input) {
    return ascii85::decode_ascii85_to_string(input);
}

// === CORRECTNESS TESTS ===

TEST(Ascii85Test, EncodeKnownValue) {
    std::string input = "hello";
    std::string expected = "<~BOu!rDZ~>"; // Standard ASCII85 for "hello"
    EXPECT_EQ(encode(input), expected);
}

TEST(Ascii85Test, DecodeKnownValue) {
    std::string encoded = "<~BOu!rDZ~>";
    std::string expected = "hello";
    EXPECT_EQ(decode(encoded), expected);
}

TEST(Ascii85Test, EncodeAnotherKnownValue) {
    std::string input = "Man";
    std::string expected = "<~9jqo~>"; // Standard ASCII85 for "Man"
    EXPECT_EQ(encode(input), expected);
}

TEST(Ascii85Test, DecodeAnotherKnownValue) {
    std::string encoded = "<~9jqo~>";
    std::string expected = "Man";
    EXPECT_EQ(decode(encoded), expected);
}

// === ROUND-TRIP TESTS ===

TEST(Ascii85Test, RoundTrip) {
    std::string input = "HelloWorld123";
    EXPECT_EQ(decode(encode(input)), input);
}

TEST(Ascii85Test, EmptyString) {
    std::string input = "";
    EXPECT_EQ(encode(input), "<~~>");
    EXPECT_EQ(decode("<~~>"), input);
}

TEST(Ascii85Test, NullBytesCompression) {
    std::string input = std::string("\0\0\0\0", 4);
    std::string encoded = encode(input);
    EXPECT_NE(encoded.find('z'), std::string::npos); // 'z' represents 4 null bytes
    EXPECT_EQ(decode(encoded), input);
}

TEST(Ascii85Test, InvalidInputThrows) {
    std::string input = "!!invalid!!";
    EXPECT_THROW(decode(input), std::runtime_error);
}

TEST(Ascii85Test, LongRoundTrip) {
    std::string input(1000, 'A');
    std::string encoded = encode(input);
    std::string decoded = decode(encoded);
    EXPECT_EQ(decoded, input);
}