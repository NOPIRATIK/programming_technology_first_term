#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <clocale>
#include <chrono>   // <-- для замеров

#ifdef _WIN32
#include <windows.h>
#endif

using Clock = std::chrono::high_resolution_clock;
using Ms = std::chrono::duration<double, std::milli>;   // миллисекунды

// ---- Приводим буфер к нижнему регистру ----
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

bool hasLetter(const char* p, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        unsigned char c = (unsigned char)p[i];
        if (std::isalpha(c)) return true;
        if (c == 0xD0 || c == 0xD1) return true;
    }
    return false;
}

// ================== ФУНКЦИИ ==================

// 1. Чтение файла
std::string readFile(const std::string& path, double& ms) {
    auto t0 = Clock::now();

    std::ifstream fin(path, std::ios::binary | std::ios::ate);
    if (!fin) return {};
    std::streamsize size = fin.tellg();
    fin.seekg(0, std::ios::beg);
    std::string text;
    text.resize((size_t)size);
    fin.read(&text[0], size);
    fin.close();

    ms = Ms(Clock::now() - t0).count();
    return text;
}

// 2. Lowercase
void lowerAll(std::string& text, double& ms) {
    auto t0 = Clock::now();
    toLowerInPlace(text);
    ms = Ms(Clock::now() - t0).count();
}

// 3. Индексация
void buildIndex(const std::string& text,
    std::unordered_map<std::string, std::vector<int>>& index,
    int& totalWords, double& ms) {
    auto t0 = Clock::now();

    index.reserve(60000);
    const char* data = text.data();
    size_t i = 0, n = text.size();
    int pos = 0;

    while (i < n) {
        while (i < n && (unsigned char)data[i] <= ' ') i++;
        size_t start = i;
        while (i < n && (unsigned char)data[i] >  ' ') i++;
        if (start == i) continue;

        size_t a = start, b = i;
        while (a < b && std::ispunct((unsigned char)data[a]))     a++;
        while (b > a && std::ispunct((unsigned char)data[b - 1])) b--;

        if (a >= b || !hasLetter(data + a, b - a)) continue;

        std::string w(data + a, b - a);
        index[w].push_back(pos);
        pos++;
    }

    totalWords = pos;
    ms = Ms(Clock::now() - t0).count();
}

// 4. Сортировка ключей
void sortKeys(std::unordered_map<std::string, std::vector<int>>& index,
    std::vector<std::string>& keys, double& ms) {
    auto t0 = Clock::now();

    keys.reserve(index.size());
    for (auto& p : index) keys.push_back(p.first);
    std::sort(keys.begin(), keys.end());

    ms = Ms(Clock::now() - t0).count();
}

// 5. Запись результата
void writeOutput(const std::string& outPath,
    std::unordered_map<std::string, std::vector<int>>& index,
    std::vector<std::string>& keys, int totalWords, double& ms) {
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

// ================== MAIN ==================
int main() {
    setlocale(LC_ALL, "");
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    const char* inPath = "C:/ОРАЛЬНЫЕ_УТЕХИ/3_курс_1_семестр/programming_technology_first_term/war_and_peace.txt";
    const char* outPath = "C:/ОРАЛЬНЫЕ_УТЕХИ/3_курс_1_семестр/programming_technology_first_term/lab1_output.txt";

    // ---- Общий таймер ----
    auto totalStart = Clock::now();

    // ---- Замеры по этапам ----
    double tRead = 0, tLower = 0, tIndex = 0, tSort = 0, tWrite = 0;

    std::string text = readFile(inPath, tRead);
    if (text.empty()) { std::cerr << "Файл не найден\n"; return 1; }

    lowerAll(text, tLower);

    std::unordered_map<std::string, std::vector<int>> index;
    int totalWords = 0;
    buildIndex(text, index, totalWords, tIndex);

    std::vector<std::string> keys;
    sortKeys(index, keys, tSort);

    writeOutput(outPath, index, keys, totalWords, tWrite);

    double tTotal = Ms(Clock::now() - totalStart).count();

    // ---- Вывод таймингов ----
    std::cout << "\n===== ВРЕМЯ ВЫПОЛНЕНИЯ =====\n";
    std::cout << "Чтение файла:     " << tRead << " мс\n";
    std::cout << "Lowercase:        " << tLower << " мс\n";
    std::cout << "Индексация:       " << tIndex << " мс\n";
    std::cout << "Сортировка:       " << tSort << " мс\n";
    std::cout << "Запись в файл:    " << tWrite << " мс\n";
    std::cout << "-----------------------------\n";
    std::cout << "ИТОГО:            " << tTotal << " мс\n";
    std::cout << "Всего слов:       " << totalWords << "\n";
    std::cout << "Уникальных:       " << index.size() << "\n";

    return 0;
}