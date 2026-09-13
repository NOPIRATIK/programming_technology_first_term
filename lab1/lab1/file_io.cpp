#include "file_io.h"
#include <fstream>
#include <chrono>

using Clock = std::chrono::high_resolution_clock;
using Ms = std::chrono::duration<double, std::milli>;

std::string readFile(const std::string& path, double& ms) {
    auto t0 = Clock::now();

    std::ifstream fin(path, std::ios::binary | std::ios::ate);
    if (!fin) { ms = 0; return {}; }

    std::streamsize size = fin.tellg();
    fin.seekg(0, std::ios::beg);

    std::string text;
    text.resize((size_t)size);
    fin.read(&text[0], size);
    fin.close();

    ms = Ms(Clock::now() - t0).count();
    return text;
}

void writeOutput(const std::string& outPath,
    WordIndex& index,
    std::vector<std::string>& keys,
    int totalWords,
    double& ms) {
    auto t0 = Clock::now();

    std::ofstream fout(outPath, std::ios::binary);
    if (!fout) { ms = 0; return; }

    fout << "Всего слов: " << totalWords
        << ", уникальных: " << index.size() << "\n";

    fout << "\n----- Подсчёт слов -----\n";
    for (auto& k : keys)
        fout << k << " - " << index[k].size() << "\n";

    fout << "\n----- Позиции слов -----\n";
    for (auto& k : keys) {
        fout << k << " - ";
        auto& v = index[k];
        for (size_t j = 0; j < v.size(); ++j) {
            if (j) fout << ", ";
            fout << v[j];
        }
        fout << "\n";
    }
    fout.close();

    ms = Ms(Clock::now() - t0).count();
}