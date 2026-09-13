#include "text_utils.h"
#include <cctype>

// Converts the given UTF-8 buffer
void toLowerInPlace(std::string& s) {
    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = (unsigned char)s[i];
        if (c < 0x80) { s[i] = (char)std::tolower(c); i++; }
        else if (c == 0xD0 && i + 1 < s.size()) {
            unsigned char c2 = (unsigned char)s[i + 1];
            if (c2 == 0x81) { s[i] = (char)0xD1; s[i + 1] = (char)0x91; }
            else if (c2 >= 0x90 && c2 <= 0x9F) { s[i + 1] = (char)(c2 + 0x20); }
            else if (c2 >= 0xA0 && c2 <= 0xAF) { s[i] = (char)0xD1; s[i + 1] = (char)(c2 - 0x20); }
            i += 2;
        }
        else i++;
    }
}

// Returns true if the range [p, p+len) contains at least one letter
bool hasLetter(const char* p, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        unsigned char c = (unsigned char)p[i];
        if (std::isalpha(c)) return true;
        if (c == 0xD0 || c == 0xD1) return true;
    }
    return false;
}