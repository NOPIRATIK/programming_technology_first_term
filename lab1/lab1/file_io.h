#pragma once
#include <string>
#include <vector>
#include "common.h"

// Reads the whole file into a string 
// Writes elapsed time. Returns empty string on failure
std::string readFile(const std::string& path, double& ms);

// Writes summary, word counts and word positions to the output file
// Writes elapsed time
void writeOutput(const std::string& outPath,
    WordIndex& index,
    std::vector<std::string>& keys,
    int totalWords,
    double& ms);