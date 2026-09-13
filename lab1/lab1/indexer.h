#pragma once
#include <string>
#include <vector>
#include "common.h"

void buildIndex(const std::string& text,
    WordIndex& index,
    int& totalWords,
    double& ms);

void sortKeys(WordIndex& index,
    std::vector<std::string>& keys,
    double& ms);