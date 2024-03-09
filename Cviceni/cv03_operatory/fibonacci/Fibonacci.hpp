#pragma once

// Fibonacci class
class Fibonacci
{
public:
    // iterator class inside Fibonacci class
    class iterator
    {
    public:
        iterator(unsigned long long int n);

        bool operator!=(const iterator &second) const;
        bool operator==(const iterator &second) const;
        iterator &operator++();
        unsigned long long int operator*() const;

    private:
        unsigned long long int current, next, count;
    };

    Fibonacci(unsigned long long int n);
    iterator begin() const;
    iterator end() const;

private:
    unsigned long long int n;
};
