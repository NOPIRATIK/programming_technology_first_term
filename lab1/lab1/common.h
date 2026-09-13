#pragma once
#include <string>
#include <vector>
#include <unordered_map>

// Shared type: word -> list of positions in text
using WordIndex = std::unordered_map<std::string, std::vector<int>>;