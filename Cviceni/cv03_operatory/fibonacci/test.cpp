#ifndef __TRAINER__
#include <cassert>
#include <sstream>
#include <iostream>

#include "Fibonacci.hpp"

int main()
{
    {
        std::ostringstream oss;
        Fibonacci fib(10);
        for (Fibonacci::iterator it = fib.begin(); it != fib.end(); ++it)
        {
            oss << *it << " ";
        }
        // jstd::cout << oss.str() << std::endl;
        assert(oss.str() == "1 1 2 3 5 8 13 21 34 55 ");
    }
    {
        std::ostringstream oss;
        const Fibonacci fib(10);
        for (Fibonacci::iterator it = fib.begin(); it != fib.end(); ++it)
        {
            oss << *it << " ";
        }
        assert(oss.str() == "1 1 2 3 5 8 13 21 34 55 ");
    }
    {
        std::ostringstream oss;
        for (auto n : Fibonacci(10))
        { // uses same things as before
            oss << n << " ";
        }
        assert(oss.str() == "1 1 2 3 5 8 13 21 34 55 ");
    }
    return 0;
}
#endif