#include "stl_tasks.h"
#include <algorithm>
#include <iterator>

bool isPrime(int n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

void task3a_squarePrimes(std::vector<int>& v) {
    std::transform(v.begin(), v.end(), v.begin(),
        [](int x) { return isPrime(x) ? x * x : x; });
}

void task3b_oddAscEvenDesc(std::vector<int>& v) {
    std::sort(v.begin(), v.end(),
        [](int a, int b) {
            bool aOdd = (a % 2 != 0);
            bool bOdd = (b % 2 != 0);
            if (aOdd != bOdd) return aOdd > bOdd;
            if (aOdd)         return a < b;
            return a > b;
        });
}

std::vector<int> task3c_uniqueInRange(const std::vector<int>& v, int lo, int hi) {
    std::vector<int> tmp;
    std::copy_if(v.begin(), v.end(), std::back_inserter(tmp),
        [lo, hi](int x) { return x >= lo && x <= hi; });
    std::sort(tmp.begin(), tmp.end());
    tmp.erase(std::unique(tmp.begin(), tmp.end()), tmp.end());
    return tmp;
}