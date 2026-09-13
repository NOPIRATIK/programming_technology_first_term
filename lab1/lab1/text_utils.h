#pragma once
#include <string>

// Converts the given UTF-8 buffer
void toLowerInPlace(std::string& s);

// Returns true if the range [p, p+len) contains at least one letter
bool hasLetter(const char* p, size_t len);