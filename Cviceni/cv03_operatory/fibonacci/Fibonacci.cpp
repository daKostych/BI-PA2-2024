#include "Fibonacci.hpp"

Fibonacci::Fibonacci(unsigned long long int n) : n(n){};

Fibonacci::iterator::iterator(unsigned long long int n) : current(1), next(1), count(1)
{
    while (count <= n)
    {
        unsigned long long int tmp = current + next;
        current = next;
        next = tmp;
        count++;
    }
};

Fibonacci::iterator Fibonacci::begin() const { return Fibonacci::iterator(0); };

Fibonacci::iterator Fibonacci::end() const { return Fibonacci::iterator(n); };

bool Fibonacci::iterator::operator!=(const iterator &second) const { return count != second.count; };

bool Fibonacci::iterator::operator==(const iterator &second) const { return count == second.count; };

Fibonacci::iterator &Fibonacci::iterator::operator++()
{
    // Fibonacci::iterator &i = *this;
    unsigned long long int tmp = current + next;
    current = next;
    next = tmp;
    count++;
    return *this;
};

unsigned long long int Fibonacci::iterator::operator*() const { return current; };