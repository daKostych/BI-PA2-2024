// next line is include guard, do not modify
#pragma once

#include <iostream>

using namespace std;

// Complex class
class Complex
{
public:
    Complex(double r, double i);
    Complex(double r);
    Complex();

    friend Complex operator+(const Complex &c1, const Complex &c2);
    friend Complex operator-(const Complex &c1, const Complex &c2);

    Complex &operator+=(const Complex &c);
    Complex &operator-=(const Complex &c);

    friend Complex operator*(const Complex &c1, const Complex &c2);
    friend Complex operator/(const Complex &c1, const Complex &c2);

    Complex &operator*=(const Complex &c);
    Complex &operator/=(const Complex &c);

    friend Complex operator-(const Complex &c);

    friend std::ostream &operator<<(std::ostream &out, const Complex &c);
    friend std::istream &operator>>(std::istream &in, Complex &c);

    friend double abs(const Complex &c);

    friend Complex sqrt(const Complex &c);

private:
    double re,
        im;
};
