#pragma once
#include <string>
#include <vector>
#include "common.h"

std::string readFile(const std::string& path, double& ms);

void writeOutput(const std::string& outPath,
    WordIndex& index,
    std::vector<std::string>& keys,
    int totalWords,
    double& ms);