#include "indexer.h"
#include "text_utils.h"
#include <cctype>
#include <algorithm>
#include <chrono>

using Clock = std::chrono::high_resolution_clock;
using Ms = std::chrono::duration<double, std::milli>;

// Parses the text buffer and fills index
// Returns the total number of words
// Writes elapsed
void buildIndex(const std::string& text,
    WordIndex& index,
    int& totalWords,
    double& ms) {
    auto t0 = Clock::now();

    index.reserve(60000);
    const char* data = text.data();
    size_t i = 0, n = text.size();
    int pos = 0;

    while (i < n) {
        // skip whitespace
        while (i < n && (unsigned char)data[i] <= ' ') i++;
        size_t start = i;
        // read until next whitespace
        while (i < n && (unsigned char)data[i] >  ' ') i++;
        if (start == i) continue;

        // trim punctuation from both ends
        size_t a = start, b = i;
        while (a < b && std::ispunct((unsigned char)data[a]))     a++;
        while (b > a && std::ispunct((unsigned char)data[b - 1])) b--;

        // skip tokens without letters (dashes, numbers, etc)
        if (a >= b || !hasLetter(data + a, b - a)) continue;

        std::string w(data + a, b - a);
        index[w].push_back(pos);
        pos++;
    }

    totalWords = pos;
    ms = Ms(Clock::now() - t0).count();
}

// Copies all keys from the unordered_map into a vector and sorts them alphabetically
// Writes elapsed time
void sortKeys(WordIndex& index,
    std::vector<std::string>& keys,
    double& ms) {
    auto t0 = Clock::now();

    keys.reserve(index.size());
    for (auto& p : index) keys.push_back(p.first);
    std::sort(keys.begin(), keys.end());

    ms = Ms(Clock::now() - t0).count();
}