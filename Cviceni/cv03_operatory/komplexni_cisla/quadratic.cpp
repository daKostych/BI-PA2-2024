#ifndef __TRAINER__
#include <cassert>
#include <sstream>
#include <cmath>
#endif

#include "Complex.hpp"

// todo: change double to Complex
bool isNearlyEqual(Complex a, Complex b)
{
    return abs(a - b) <= (abs(a) + abs(b)) * 1e-6;
}

// todo: change double to Complex
void solve(const Complex &a, const Complex &b, const Complex &c, std::ostream &out)
{
    auto _b2 = b * b;
    auto _4ac = 4 * a * c;
    if (isNearlyEqual(_b2, _4ac))
    {
        out << "Pouze jedno reseni: " << (-b / 2 / a);
    }
    else
    {
        out << "Dve reseni: " << ((-b - sqrt(_b2 - _4ac)) / 2 / a) << " a " << ((-b + sqrt(_b2 - _4ac)) / 2 / a);
    }
}

#ifndef __TRAINER__
int main()
{
    {
        std::ostringstream oss;
        solve(1, 4, 4, oss);
        // std::cout << oss.str() << std::endl;
        assert(oss.str() == "Pouze jedno reseni: [-2, 0]");
    }
    {
        std::ostringstream oss;
        solve(1, 0, -4, oss);
        // std::cout << oss.str() << std::endl;
        assert(oss.str() == "Dve reseni: [-2, 0] a [2, 0]");
    }
    {
        std::ostringstream oss;
        solve(1, -4, 8, oss);
        // std::cout << oss.str() << std::endl;
        assert(oss.str() == "Dve reseni: [2, 2] a [2, -2]");
    }
    {
        std::ostringstream oss;
        solve(1, 1, 4, oss);
        // std::cout << oss.str() << std::endl;
        assert(oss.str() == "Dve reseni: [-0.5, -1.93649] a [-0.5, 1.93649]");
    }
    return 0;
}
#endif