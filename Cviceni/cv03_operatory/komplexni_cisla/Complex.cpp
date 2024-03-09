#include <cmath>
#include "Complex.hpp"

using namespace std;

Complex::Complex(double r, double i) : re(r), im(i){};
Complex::Complex(double r) : re(r), im(0){};
Complex::Complex() : re(0), im(0){};

Complex operator+(const Complex &c1, const Complex &c2) { return {c1.re + c2.re, c1.im + c2.im}; };
Complex operator-(const Complex &c1, const Complex &c2) { return {c1.re - c2.re, c1.im - c2.im}; };

Complex &Complex::operator+=(const Complex &c)
{
    *this = *this + c;
    return *this;
};

Complex &Complex::operator-=(const Complex &c)
{
    *this = *this - c;
    return *this;
};

Complex operator*(const Complex &c1, const Complex &c2) { return {c1.re * c2.re - c1.im * c2.im, c1.re * c2.im + c1.im * c2.re}; };
Complex operator/(const Complex &c1, const Complex &c2)
{
    double denominator = c2.re * c2.re + c2.im * c2.im;
    double realPart = (c1.re * c2.re + c1.im * c2.im) / denominator;
    double imagPart = (c1.im * c2.re - c1.re * c2.im) / denominator;

    return {realPart, imagPart};
};

Complex &Complex::operator*=(const Complex &c)
{
    *this = *this * c;
    return *this;
};

Complex &Complex::operator/=(const Complex &c)
{
    *this = *this / c;
    return *this;
};

Complex operator-(const Complex &c) { return {-c.re, -c.im}; };

std::ostream &operator<<(std::ostream &out, const Complex &c)
{
    return out << "[" << c.re << ", " << c.im << "]";
};

std::istream &operator>>(std::istream &in, Complex &c)
{
    // double real, imag;
    char bracket1, comma, bracket2;

    if (!(in >> bracket1 >> c.re >> comma >> c.im >> bracket2) || bracket1 != '[' || bracket2 != ']' || comma != ',')
        in.setstate(std::ios::failbit);

    return in;
};

double abs(const Complex &c) { return std::sqrt(c.re * c.re + c.im * c.im); };

Complex sqrt(const Complex &c)
{
    double magnitude = std::sqrt(abs(c));
    double angle = std::atan2(c.im, c.re) / 2.0; // Half of the angle in radians

    // Use polar coordinates to calculate the square root
    return {magnitude * std::cos(angle), magnitude * std::sin(angle)};
};