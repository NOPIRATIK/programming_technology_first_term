#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <clocale>
#include <chrono>
#include <iterator>
#include <limits>

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max
#undef tolower
#undef toupper
#endif

using Clock = std::chrono::high_resolution_clock;
using Ms = std::chrono::duration<double, std::milli>;

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

// Приведение строки к нижнему регистру (с поддержкой кириллицы UTF-8)
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

bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; ++i) if (n % i == 0) return false;
    return true;
}

void printVector(const std::vector<int>& v, const std::string& title = "") {
    if (!title.empty()) std::cout << title;
    if (v.empty()) { std::cout << "(пусто)\n"; return; }
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';
}

std::vector<int> inputVector() {
    std::vector<int> v;
    int n, x;
    std::cout << "Введите количество элементов: ";
    std::cin >> n;
    std::cout << "Введите " << n << " чисел через пробел: ";
    for (int i = 0; i < n; ++i) { std::cin >> x; v.push_back(x); }
    return v;
}

// ==================== ЯДРО ИНДЕКСАЦИИ ====================

// Разбор текста → карта: слово → список позиций
void buildIndex(const std::string& text,
    std::unordered_map<std::string, std::vector<int>>& index,
    int& totalWords) {
    index.clear();
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
}

// Получить отсортированный список ключей
std::vector<std::string> sortedKeys(
    const std::unordered_map<std::string, std::vector<int>>& index) {
    std::vector<std::string> keys;
    keys.reserve(index.size());
    for (auto& p : index) keys.push_back(p.first);
    std::sort(keys.begin(), keys.end());
    return keys;
}

// ==================== ЗАДАНИЕ 1: ПОДСЧЁТ СЛОВ ====================

void outputWordCounts(const std::unordered_map<std::string, std::vector<int>>& index,
    const std::vector<std::string>& keys,
    std::ostream& os = std::cout) {
    for (auto& k : keys)
        os << k << " - " << index.at(k).size() << "\n";
}

void runWordCount() {
    const char* inPath = "C:/ОРАЛЬНЫЕ_УТЕХИ/3_курс_1_семестр/programming_technology_first_term/war_and_peace.txt";

    std::ifstream fin(inPath, std::ios::binary | std::ios::ate);
    if (!fin) { std::cerr << "Файл не найден: " << inPath << "\n"; return; }

    std::streamsize size = fin.tellg();
    fin.seekg(0, std::ios::beg);
    std::string text((size_t)size, '\0');
    fin.read(&text[0], size);
    fin.close();

    toLowerInPlace(text);

    std::unordered_map<std::string, std::vector<int>> index;
    int totalWords = 0;
    buildIndex(text, index, totalWords);
    auto keys = sortedKeys(index);

    std::cout << "Всего слов: " << totalWords
        << ", уникальных: " << index.size() << "\n\n";
    std::cout << "----- Подсчёт слов -----\n";
    outputWordCounts(index, keys);
}

// ==================== ЗАДАНИЕ 2: ИНДЕКСАЦИЯ ПОЗИЦИЙ ====================

void outputWordPositions(const std::unordered_map<std::string, std::vector<int>>& index,
    const std::vector<std::string>& keys,
    std::ostream& os = std::cout) {
    for (auto& k : keys) {
        os << k << " - ";
        const auto& v = index.at(k);
        for (size_t j = 0; j < v.size(); ++j) {
            if (j) os << ", ";
            os << "позиция №" << v[j];
        }
        os << "\n";
    }
}

void runWordPositions() {
    const char* inPath = "C:/ОРАЛЬНЫЕ_УТЕХИ/3_курс_1_семестр/programming_technology_first_term/war_and_peace.txt";

    std::ifstream fin(inPath, std::ios::binary | std::ios::ate);
    if (!fin) { std::cerr << "Файл не найден: " << inPath << "\n"; return; }

    std::streamsize size = fin.tellg();
    fin.seekg(0, std::ios::beg);
    std::string text((size_t)size, '\0');
    fin.read(&text[0], size);
    fin.close();

    toLowerInPlace(text);

    std::unordered_map<std::string, std::vector<int>> index;
    int totalWords = 0;
    buildIndex(text, index, totalWords);
    auto keys = sortedKeys(index);

    std::cout << "Всего слов: " << totalWords
        << ", уникальных: " << index.size() << "\n\n";
    std::cout << "----- Позиции слов -----\n";
    outputWordPositions(index, keys);
}

// ==================== ЗАДАНИЕ 3: STL algorithm ====================

// 3a. Простые числа в квадрат
void squarePrimes(std::vector<int>& v) {
    std::transform(v.begin(), v.end(), v.begin(), [](int x) {
        return isPrime(x) ? x * x : x;
        });
}

// 3b. Нечётные ↑, затем чётные ↓
void sortOddAscEvenDesc(std::vector<int>& v) {
    std::sort(v.begin(), v.end(), [](int a, int b) {
        if (a % 2 != b % 2) return (a % 2) > (b % 2);
        if (a % 2 != 0)     return a < b;
        return a > b;
        });
}

// 3c. Уникальные элементы в диапазоне
std::vector<int> uniqueInRange(const std::vector<int>& v, int low, int high) {
    std::vector<int> result;
    std::copy_if(v.begin(), v.end(), std::back_inserter(result),
        [low, high](int x) { return x >= low && x <= high; });
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}

void task3a() {
    std::cout << "\n--- 3a. Простые числа в квадрат ---\n";
    std::vector<int> v = inputVector();
    printVector(v, "Исходный вектор:       ");
    squarePrimes(v);
    printVector(v, "После преобразования:  ");
}

void task3b() {
    std::cout << "\n--- 3b. Сортировка (нечётные ↑, чётные ↓) ---\n";
    std::vector<int> v = inputVector();
    printVector(v, "Исходный вектор:   ");
    sortOddAscEvenDesc(v);
    printVector(v, "После сортировки:  ");
}

void task3c() {
    std::cout << "\n--- 3c. Уникальные элементы в диапазоне ---\n";
    std::vector<int> v = inputVector();
    int low, high;
    std::cout << "Введите диапазон (low high): ";
    std::cin >> low >> high;
    printVector(v, "Исходный вектор:   ");
    std::vector<int> unique = uniqueInRange(v, low, high);
    std::cout << "Диапазон: [" << low << ", " << high << "]\n";
    printVector(unique, "Уникальные:        ");
}

// ==================== ГЛАВНОЕ МЕНЮ ====================

int main() {
    setlocale(LC_ALL, "Russian");
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    while (true) {
        std::cout << "\n========================================\n";
        std::cout << "            Лабораторная работа №1\n";
        std::cout << "========================================\n";
        std::cout << "--- Задание 1: Подсчёт слов в файле ---\n";
        std::cout << "  1. Подсчёт уникальных слов (слово - счётчик)\n";
        std::cout << "--- Задание 2: Индексация позиций ---\n";
        std::cout << "  2. Позиции слов в файле (слово - позиция №...)\n";
        std::cout << "--- Задание 3: STL algorithm ---\n";
        std::cout << "  3. 3a. Простые числа в квадрат\n";
        std::cout << "  4. 3b. Сортировка (нечётные ↑, чётные ↓)\n";
        std::cout << "  5. 3c. Уникальные числа в диапазоне\n";
        std::cout << "--- Выход ---\n";
        std::cout << "  0. Выход\n";
        std::cout << "Выберите пункт: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка ввода. Попробуйте снова.\n";
            continue;
        }

        switch (choice) {
        case 1: runWordCount();     break;
        case 2: runWordPositions(); break;
        case 3: task3a();           break;
        case 4: task3b();           break;
        case 5: task3c();           break;
        case 0:
            std::cout << "Выход из программы.\n";
            return 0;
        default:
            std::cout << "Нет такого пункта. Попробуйте снова.\n";
        }
    }
}