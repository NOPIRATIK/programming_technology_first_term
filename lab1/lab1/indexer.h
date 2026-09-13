#pragma once
#include <string>
#include <vector>
#include "common.h"

// Parses the text buffer and fills index
// Returns the total number of words
// Writes elapsed time
void buildIndex(const std::string& text,
    WordIndex& index,
    int& totalWords,
    double& ms);

// Copies all keys from the unordered_map into a vector and sorts them alphabetically
// Writes elapsed time
void sortKeys(WordIndex& index,
    std::vector<std::string>& keys,
    double& ms);