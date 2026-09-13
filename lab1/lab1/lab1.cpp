#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <clocale>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

#include "common.h"
#include "file_io.h"
#include "indexer.h"
#include "text_utils.h"
#include "stl_tasks.h"

using Clock = std::chrono::high_resolution_clock;
using Ms = std::chrono::duration<double, std::milli>;

// ---------- вспомогательное ----------

std::vector<int> inputVector() {
    size_t n;
    std::cout << "Размер массива: ";
    std::cin >> n;

    std::vector<int> v(n);
    std::cout << "Введите " << n << " чисел: ";
    for (auto& x : v) std::cin >> x;
    return v;
}

void printVector(const std::vector<int>& v) {
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
}

// Читает диапазон в любом формате: "1 3", "[1, 3]", "1,3", "1..3"
bool readRange(int& lo, int& hi) {
    std::string line;
    std::getline(std::cin, line);

    // все нецифровые (кроме минуса) → пробел
    for (char& c : line)
        if (!std::isdigit((unsigned char)c) && c != '-')
            c = ' ';

    std::istringstream iss(line);
    return (bool)(iss >> lo >> hi);
}

// ---------- анализ текста ----------

void runWordAnalysis() {
    const char* inPath = "C:/ОРАЛЬНЫЕ_УТЕХИ/3_курс_1_семестр/programming_technology_first_term/war_and_peace.txt";
    const char* outPath = "C:/ОРАЛЬНЫЕ_УТЕХИ/3_курс_1_семестр/programming_technology_first_term/lab1_output.txt";

    auto totalStart = Clock::now();

    double tRead = 0, tLower = 0, tIndex = 0, tSort = 0, tWrite = 0;

    std::string text = readFile(inPath, tRead);
    if (text.empty()) { std::cerr << "Файл не найден\n"; return; }

    { auto t0 = Clock::now(); toLowerInPlace(text); tLower = Ms(Clock::now() - t0).count(); }

    WordIndex index;
    int totalWords = 0;
    buildIndex(text, index, totalWords, tIndex);

    std::vector<std::string> keys;
    sortKeys(index, keys, tSort);

    writeOutput(outPath, index, keys, totalWords, tWrite);

    double tTotal = Ms(Clock::now() - totalStart).count();

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
}

// ---------- меню ----------

void printMenu() {
    std::cout << "\n========== МЕНЮ ==========\n"
        << "1. Анализ текста (Война и мир)\n"
        << "2. Задание 3a: квадраты простых чисел\n"
        << "3. Задание 3b: нечётные по возрастанию, чётные по убыванию\n"
        << "4. Задание 3c: уникальные в диапазоне\n"
        << "0. Выход\n"
        << "Выбор: ";
}

int main() {
    setlocale(LC_ALL, "");
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    int choice;
    while (true) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        std::cin.ignore(10000, '\n');   // съедаем '\n' после выбора

        if (choice == 0) break;

        switch (choice) {
        case 1:
            runWordAnalysis();
            break;

        case 2: {
            std::vector<int> v = inputVector();
            std::cin.ignore(10000, '\n');   // <-- заодно
            task3a_squarePrimes(v);
            std::cout << "Результат: ";
            printVector(v);
            break;
        }

        case 3: {
            std::vector<int> v = inputVector();
            std::cin.ignore(10000, '\n');   // <-- заодно
            task3b_oddAscEvenDesc(v);
            std::cout << "Результат: ";
            printVector(v);
            break;
        }

        case 4: {
            std::vector<int> v = inputVector();

            // ← ВОТ ЭТА СТРОКА: убираем '\n' после ввода чисел
            std::cin.ignore(10000, '\n');

            int lo, hi;
            std::cout << "Диапазон (например: 1 3 или [1, 3]): ";
            if (!readRange(lo, hi)) {
                std::cout << "Не удалось прочитать диапазон.\n";
                break;
            }

            std::vector<int> r = task3c_uniqueInRange(v, lo, hi);
            std::cout << "Результат: ";
            printVector(r);
            break;
        }

        default:
            std::cout << "Неверный выбор.\n";
        }
    }
    return 0;
}