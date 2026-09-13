#include "stl_tasks.h"
#include <algorithm>
#include <iterator>

// Returns true if n is a prime number
bool isPrime(int n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

// Task 3a: replaces every prime number in the vector with its square
void task3a_squarePrimes(std::vector<int>& v) {
    std::transform(v.begin(), v.end(), v.begin(),
        [](int x) { return isPrime(x) ? x * x : x; });
}

// Task 3b: sorts the vector so that odd numbers come first, followed by even numbers
void task3b_oddAscEvenDesc(std::vector<int>& v) {
    std::sort(v.begin(), v.end(),
        [](int a, int b) {
            bool aOdd = (a % 2 != 0);
            bool bOdd = (b % 2 != 0);
            if (aOdd != bOdd) return aOdd > bOdd;   // odds before evens
            if (aOdd)         return a < b;         // odds ascending
            return a > b;                            // evens descending
        });
}

// Task 3c: returns a sorted vector of unique elements within [lo, hi]
std::vector<int> task3c_uniqueInRange(const std::vector<int>& v, int lo, int hi) {
    std::vector<int> tmp;

    // 1) keep only elements inside [lo, hi]
    std::copy_if(v.begin(), v.end(), std::back_inserter(tmp),
        [lo, hi](int x) { return x >= lo && x <= hi; });

    // 2) sort
    std::sort(tmp.begin(), tmp.end());

    // 3) remove consecutive duplicates
    tmp.erase(std::unique(tmp.begin(), tmp.end()), tmp.end());

    return tmp;
}