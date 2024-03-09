#ifndef __TRAINER__
#include <cassert>
#include <sstream>

#include "Complex.hpp"

int main()
{
    Complex a(3, 4), b(5, 6);

    // *
    {
        std::ostringstream os;
        Complex c = a * b;
        Complex d = c * 3;
        Complex e = 7 * d;
        Complex f = Complex(1, 2) * Complex(11, 12);
        os << f * e;
        assert(os.str() == "[-24675, -16800]");
    }

    // /
    {
        std::ostringstream os;
        Complex c = a / b;
        Complex d = c / 3;
        Complex e = 7 / d;
        Complex f = Complex(1, 2) / Complex(11, 12);
        os << f / e;
        assert(os.str() == "[0.00396212, 0.00135507]");
    }

    // *=
    {
        std::ostringstream os;
        Complex c = a;
        c *= Complex(3, 5);
        (c *= 7) *= Complex(12, 11);
        os << c;
        assert(os.str() == "[-3003, 1421]");
    }

    // /=
    {
        std::ostringstream os;
        Complex c = a;
        c /= Complex(3, 5);
        (c /= 7) /= Complex(12, 11);
        os << c;
        assert(os.str() == "[0.00499445, -0.00562867]");
    }

    // -
    {
        std::ostringstream os;
        Complex c = -a;
        os << c << -Complex(29, 17) << -c;
        assert(os.str() == "[-3, -4][-29, -17][3, 4]");
    }

    // >>
    {
        std::istringstream is("     [ 123  , 456 ]  [ 666,    420][-11,-22][-1, 3]  [   5   ,   -7   ]");
        std::ostringstream os;
        Complex c(0);
        while (is >> c)
            os << c;
        assert(os.str() == "[123, 456][666, 420][-11, -22][-1, 3][5, -7]");
    }
}
#endif