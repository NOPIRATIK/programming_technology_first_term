#pragma once
#include <vector>

// Task 3a: replaces every prime number in the vector with its square
void task3a_squarePrimes(std::vector<int>& v);

// Task 3b: sorts the vector so that odd numbers come first ,followed by even numbers
void task3b_oddAscEvenDesc(std::vector<int>& v);

// Task 3c: returns a sorted vector of unique elements within [lo, hi]
std::vector<int> task3c_uniqueInRange(const std::vector<int>& v, int lo, int hi);

// Returns true if n is a prime number.
bool isPrime(int n);