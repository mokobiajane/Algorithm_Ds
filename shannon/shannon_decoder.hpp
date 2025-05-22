#pragma once
#include <string>

void decodeFile(const std::string& compressedFile,
                const std::string& dictFile,
                const std::string& outputFile);