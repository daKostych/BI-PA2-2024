#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <compare>
#include <stdexcept>
#endif /* __PROGTEST__ */

using namespace std;

struct Number
{
    string digits = "0";
    bool isNegative = false;
};

class CBigInt
{
  public:
    // default constructor
    CBigInt() = default;

    // copying/assignment/destruction
    CBigInt & operator=(const CBigInt & second)
    {
        this->num.digits = second.num.digits;
        this->num.isNegative = second.num.isNegative;
        return *this;
    };
    CBigInt & operator=(const string & second)
    {
        if(second[0] == '-')
        {
            for(size_t i = 1; i < second.size(); i ++)
                if (!isdigit(second[i]))
                    throw invalid_argument("Invalid argument.");
            this->num.isNegative = true;
            num.digits = second.substr(1);
            return *this;
        }
        else
        {
            for(char i : second)
                if (!isdigit(i))
                    throw invalid_argument("Invalid argument.");
            this->num.isNegative = false;
            num.digits = second;
            return *this;
        }
    };
    CBigInt & operator=(const int second)
    {
        second < 0 ? this->num.isNegative = true : this->num.isNegative = false;
        this->num.digits = to_string(abs(second));
        return *this;
    };


    // int constructor
    CBigInt(const int number)
    {
        if(number < 0)
            num.isNegative = true;
        num.digits = to_string(abs(number));
    };

    // string constructor
    CBigInt(const string & number)
    {
        if(number[0] == '-')
        {
            for(size_t i = 1; i < number.size(); i ++)
                if (!isdigit(number[i]))
                    throw invalid_argument("Invalid argument.");
            num.isNegative = true;
            num.digits = number.substr(1);

        }
        else
        {
            for(char i : number)
            if (!isdigit(i))
                throw invalid_argument("Invalid argument.");
            num.digits = number;
        }
    };

    // operator +, any combination {CBigInt/int/string} + {CBigInt/int/string}
    CBigInt operator+ (const CBigInt & second) const
    {
        string newNumber;
        int carry = 0;
        if((!this->num.isNegative && !second.num.isNegative) || (this->num.isNegative && second.num.isNegative))
        {
            if(this->num.digits.size() >= second.num.digits.size())
            {
                for(size_t i = 0; i < second.num.digits.size(); i++)
                {
                    string digit1(1, this->num.digits[this->num.digits.size() - i - 1]);
                    string digit2(1, second.num.digits[second.num.digits.size() - i - 1]);
                    newNumber += to_string((stoi(digit1) + stoi(digit2) + carry) % 10);
                    carry = (stoi(digit1) + stoi(digit2) + carry) / 10;
                }
                for(ssize_t i = this->num.digits.size() - second.num.digits.size() - 1; i >= 0; i--)
                {
                    string digit1(1, this->num.digits[i]);
                    newNumber += to_string((stoi(digit1) + carry) % 10);
                    carry = (stoi(digit1) + carry) / 10;
                }
                if(carry != 0)
                    newNumber += to_string(carry);
                if(this->num.isNegative)
                    newNumber += '-';
                reverse(newNumber.begin(), newNumber.end());
            }
            else
            {
                for(size_t i = 0; i < this->num.digits.size(); i++)
                {
                    string digit1(1, this->num.digits[this->num.digits.size() - i - 1]);
                    string digit2(1, second.num.digits[second.num.digits.size() - i - 1]);
                    newNumber += to_string((stoi(digit1) + stoi(digit2) + carry) % 10);
                    carry = (stoi(digit1) + stoi(digit2) + carry) / 10;
                }
                for(int i = second.num.digits.size() - this->num.digits.size() - 1; i >= 0; i--)
                {
                    string digit1(1, second.num.digits[i]);
                    newNumber += to_string((stoi(digit1) + carry) % 10);
                    carry = (stoi(digit1) + carry) / 10;
                }
                if(carry != 0)
                    newNumber += to_string(carry);
                if(this->num.isNegative)
                    newNumber += '-';
                reverse(newNumber.begin(), newNumber.end());
            }
        }
        else
        {
            if(this->num.digits.size() > second.num.digits.size())
            {
                newNumber = minus(*this, second);
                if (this->num.isNegative)
                    newNumber += "-";
            }
            else if (this->num.digits.size() < second.num.digits.size())
            {
                newNumber = minus(second, *this);
                if (second.num.isNegative)
                    newNumber += "-";
            }
            else
            {
                if (this->num.digits == second.num.digits)
                    newNumber = "0";
                if (this->num.digits < second.num.digits)
                {
                    newNumber = minus(second, *this);
                    if (second.num.isNegative)
                        newNumber += "-";
                }
                else
                {
                    newNumber = minus(*this, second);
                    if (this->num.isNegative)
                        newNumber += "-";
                }
            }
            reverse(newNumber.begin(), newNumber.end());
            return newNumber;
        }

        return {newNumber};
    };
    CBigInt operator+ (const string & second) const { return *this + CBigInt(second); };
    friend CBigInt operator+ (const int first, const CBigInt & second) { return (CBigInt(first) + second); };
    friend CBigInt operator+ (string first, const CBigInt & second) { return (CBigInt(std::move(first)) + second); };

    // operator *, any combination {CBigInt/int/string} * {CBigInt/int/string}
    CBigInt operator* (const CBigInt & second) const
    {
        CBigInt toReturn;

        if (this->num.digits == "0" || second.num.digits == "0")
            return {0};

        if (this->num.digits.size() >= second.num.digits.size())
            mul(toReturn, *this, second);
        else
            mul(toReturn, second, *this);

        if (this->num.isNegative || second.num.isNegative)
            toReturn.num.isNegative = true;

        if (this->num.isNegative && second.num.isNegative)
            toReturn.num.isNegative = false;

        return toReturn;
    };
    CBigInt operator* (const string & second) const { return *this * CBigInt(second); }
    friend CBigInt operator* (const int first, const CBigInt & second) { return (CBigInt(first) * second); };
    friend CBigInt operator* (const string & first, const CBigInt & second) { return (CBigInt(first) * second); };

    // operator +=, any of {CBigInt/int/string}
    CBigInt & operator+=(const CBigInt & second)
    {
        *this = *this + second;
        return *this;
    };
    CBigInt & operator+=(const int second)
    {
        *this = *this + CBigInt(second);
        return *this;
    };
    CBigInt & operator+=(const string & second)
    {
        *this = *this + CBigInt(second);
        return *this;
    };

    // operator *=, any of {CBigInt/int/string}
    CBigInt & operator*=(const CBigInt & second)
    {
        *this = *this * second;
        return *this;
    };
    CBigInt & operator*=(const int second)
    {
        *this = *this * CBigInt(second);
        return *this;
    };
    CBigInt & operator*=(const string & second)
    {
        *this = *this * CBigInt(second);
        return *this;
    };

    // comparison operators, any combination {CBigInt/int/string} {<,<=,>,>=,==,!=} {CBigInt/int/string}
    bool operator<(const CBigInt & second) const
    {
       if (this->num.isNegative && !second.num.isNegative)
           return true;
       else if(!this->num.isNegative && second.num.isNegative)
           return false;
       else if (!this->num.isNegative && !second.num.isNegative)
       {
           if (this->num.digits.size() < second.num.digits.size())
               return true;
           else if (this->num.digits.size() > second.num.digits.size())
               return false;
           else
           {
               for (size_t i = 0; i < this->num.digits.size(); i++)
               {
                   if (this->num.digits[i] == second.num.digits[i])
                       continue;
                   if (this->num.digits[i] < second.num.digits[i])
                       return true;
                   else return false;
               }
           }
       }
       else
       {
           if (this->num.digits.size() < second.num.digits.size())
               return false;
           else if (this->num.digits.size() > second.num.digits.size())
               return true;
           else
           {
               for (size_t i = 0; i < this->num.digits.size(); i++)
               {
                   if (this->num.digits[i] == second.num.digits[i])
                       continue;
                   if (this->num.digits[i] < second.num.digits[i])
                       return false;
                   else return true;
               }
           }
       }
       return false;
    };
    bool operator<(const string & second) const { return *this < CBigInt(second); };
    friend bool operator<(const int first, const CBigInt & second) { return CBigInt(first) < second; };
    friend bool operator<(const string & first, const CBigInt & second) { return CBigInt(first) < second; };

    bool operator>(const CBigInt & second) const { return (!(*this < second) && !(*this == second)); };
    bool operator>(const string & second) const { return (!(*this < CBigInt(second)) && !(*this == CBigInt(second))); };
    friend bool operator>(const int first, const CBigInt & second) { return (!(CBigInt(first) < second) && !(CBigInt(first) == second)); };
    friend bool operator>(const string & first, const CBigInt & second) { return (!(CBigInt(first) < second) && !(CBigInt(first) == second)); };

    bool operator==(const CBigInt & second) const
    {
        if ((this->num.isNegative && !second.num.isNegative) || (!this->num.isNegative && second.num.isNegative))
            return false;
        else if (this->num.digits.size() != second.num.digits.size())
            return false;
        else
        {
            for (size_t i = 0; i < this->num.digits.size(); i++)
                if (this->num.digits[i] != second.num.digits[i])
                    return false;
            return true;
        }
    };
    bool operator==(const string & second) const { return *this == CBigInt(second); };
    friend bool operator==(const int first, const CBigInt & second) { return CBigInt(first) == second; };
    friend bool operator==(const string & first, const CBigInt & second) { return CBigInt(first) == second; };

    bool operator!=(const CBigInt & second) const { return !(*this == second); };
    bool operator!=(const string & second) const { return !(*this == CBigInt(second)); };
    friend bool operator!=(const int first, const CBigInt & second) { return !(CBigInt(first) == second); };
    friend bool operator!=(const string & first, const CBigInt & second) { return !(CBigInt(first) == second); };

    bool operator<=(const CBigInt & second) const { return (*this < second) || (*this == second); };
    bool operator<=(const string & second) const { return (*this < CBigInt(second)) || (*this == CBigInt(second)); };
    friend bool operator<=(const int first, const CBigInt & second) { return (CBigInt(first) < second) || (CBigInt(first) == second); };
    friend bool operator<=(const string & first, const CBigInt & second) { return (CBigInt(first) < second) || (CBigInt(first) == second); };

    bool operator>=(const CBigInt & second) const { return !(*this < second) || (*this == second); };
    bool operator>=(const string & second) const { return !(*this < CBigInt(second)) || (*this == CBigInt(second)); };
    friend bool operator>=(const int first, const CBigInt & second) { return !(CBigInt(first) < second) || (CBigInt(first) == second); };
    friend bool operator>=(const string & first, const CBigInt & second) { return !(CBigInt(first) < second) || (CBigInt(first) == second); };

    // output operator <<
    friend ostream & operator<<(ostream & out, const CBigInt & number)
    {
        if (number.num.isNegative)
            out << '-';
        size_t firstNonZero = number.num.digits.find_first_not_of('0');
        if (firstNonZero == string::npos)
            out << "0";
        else
            out << number.num.digits.substr(firstNonZero);
        return out;
    };
    friend ostream & operator<<(ostream & out, const string & number) { return out << CBigInt(number); };

    // input operator >>
    friend istream & operator>>(istream & in, CBigInt & number)
    {
        bool negative = false;
        char digit;
        string newNumber;

        in >> ws;
        if (in.peek() != '-' && !isdigit(in.peek()))
        {
            in.setstate(ios::failbit);
            return in;
        }

        if (in.peek() == '-')
        {
            in.get(digit);
            negative = true;
        }

        if (!isdigit(in.peek()))
        {
            in.setstate(ios::failbit);
            return in;
        }

        while(isdigit(in.peek()))
        {
            in.get(digit);
            newNumber += digit;
        }

        number.num.digits = newNumber;
        number.num.isNegative = negative;
        return in;
    };

  private:
    Number num;

    static string minus(const CBigInt & first, const CBigInt & second)
    {
        bool carry = false;
        string newNumber;

        for (size_t i = 0; i < second.num.digits.size(); i++)
        {
            string d1(1, first.num.digits[first.num.digits.size() - i - 1]);
            int digit1 = stoi(d1);
            if(carry)
                digit1 = (digit1 - 1) % 10;
            string d2(1, second.num.digits[second.num.digits.size() - i - 1]);
            int digit2 = stoi(d2);
            if((digit1 < digit2))
            {
                digit1 = 10 + digit1;
                carry = true;
            }
            else if (carry && digit1 == 9)
                carry = true;
            else
                carry = false;
            newNumber += to_string(digit1 - digit2);
        }
        for (size_t i = 0; i < first.num.digits.size() - second.num.digits.size(); i++)
        {
            string d(1, first.num.digits[first.num.digits.size() - second.num.digits.size() - 1 - i]);
            int digit = stoi(d);
            if (carry)
                digit = (digit - 1) % 10;
            if (digit == 9 && carry)
                carry = true;
            else
                carry = false;
            newNumber += to_string(digit);
        }

        //reverse(newNumber.begin(), newNumber.end());
        return newNumber;
    };

    static void mul(CBigInt & toReturn, const CBigInt & first, const CBigInt & second)
    {
        size_t sum, multiply;
        ssize_t firstSize = first.num.digits.size(), secondSize = second.num.digits.size();
        string newNumber(first.num.digits.size() + second.num.digits.size(), '0');

        for (ssize_t i = 0; i < firstSize; i++) {
            for (ssize_t j = 0; j < secondSize; j++) {
                multiply = (first.num.digits[firstSize - i - 1] - '0') * (second.num.digits[secondSize - j - 1] - '0');
                sum = multiply + (newNumber[firstSize - i + secondSize - j - 1] - '0');
                newNumber[firstSize - i + secondSize - j - 2] += sum / 10;
                newNumber[firstSize - i + secondSize - j - 1] = (sum % 10) + '0';
            }
        }

        size_t position = newNumber.find_first_not_of('0');
        position == string::npos ? toReturn.num.digits = "0"
                                 : toReturn.num.digits = newNumber.substr(position);
    };
};

#ifndef __PROGTEST__

static bool equal ( const CBigInt & x, const char val [] )
{
  std::ostringstream oss;
  oss << x;
  return oss . str () == val;
}

static bool equalHex ( const CBigInt & x, const char val [] )
{
  return true; // hex output is needed for bonus tests only
  // std::ostringstream oss;
  // oss << std::hex << x;
  // return oss . str () == val;
}
int main ()
{
    CBigInt a, b;
    std::istringstream is;
    a = 10;
    a += 20;
    assert ( equal ( a, "30" ) );
    a *= 5;
    assert ( equal ( a, "150" ) );
    b = a + 3;
    assert ( equal ( b, "153" ) );
    b = a * 7;
    assert ( equal ( b, "1050" ) );
    assert ( equal ( a, "150" ) );
    assert ( equalHex ( a, "96" ) );

    a = 10;
    a += -20;
    assert ( equal ( a, "-10" ) );
    a *= 5;
    assert ( equal ( a, "-50" ) );
    b = a + 73;
    assert ( equal ( b, "23" ) );
    b = a * -7;
    assert ( equal ( b, "350" ) );
    assert ( equal ( a, "-50" ) );
    assert ( equalHex ( a, "-32" ) );

    a = "12345678901234567890";
    a += "-99999999999999999999";
    assert ( equal ( a, "-87654321098765432109" ) );
    a *= "54321987654321987654";
    assert ( equal ( a, "-4761556948575111126880627366067073182286" ) );
    a *= 0;
    assert ( equal ( a, "0" ) );
    a = 10;
    b = a + "400";
    assert ( equal ( b, "410" ) );
    b = a * "15";
    assert ( equal ( b, "150" ) );
    assert ( equal ( a, "10" ) );
    assert ( equalHex ( a, "a" ) );


    is . clear ();
    is . str ( " 1234" );
    assert ( is >> b );
    assert ( equal ( b, "1234" ) );
    is . clear ();
    is . str ( " 12 34" );
    assert ( is >> b );
    assert ( equal ( b, "12" ) );
    is . clear ();
    is . str ( "999z" );
    assert ( is >> b );
    assert ( equal ( b, "999" ) );
    is . clear ();
    is . str ( "abcd" );
    assert ( ! ( is >> b ) );
    is . clear ();
    is . str ( "- 758" );
    assert ( ! ( is >> b ) );
    a = 42;

    try
    {
      a = "-xyz";
      assert ( "missing an exception" == nullptr );
    }
    catch ( const std::invalid_argument & e )
    {
      assert ( equal ( a, "42" ) );
    }


    a = "73786976294838206464";
    assert ( equal ( a, "73786976294838206464" ) );
    assert ( equalHex ( a, "40000000000000000" ) );
    assert ( a < "1361129467683753853853498429727072845824" );
    assert ( a <= "1361129467683753853853498429727072845824" );
    assert ( ! ( a > "1361129467683753853853498429727072845824" ) );
    assert ( ! ( a >= "1361129467683753853853498429727072845824" ) );
    assert ( ! ( a == "1361129467683753853853498429727072845824" ) );
    assert ( a != "1361129467683753853853498429727072845824" );
    assert ( ! ( a < "73786976294838206464" ) );
    assert ( a <= "73786976294838206464" );
    assert ( ! ( a > "73786976294838206464" ) );
    assert ( a >= "73786976294838206464" );
    assert ( a == "73786976294838206464" );
    assert ( ! ( a != "73786976294838206464" ) );
    assert ( a < "73786976294838206465" );
    assert ( a <= "73786976294838206465" );
    assert ( ! ( a > "73786976294838206465" ) );
    assert ( ! ( a >= "73786976294838206465" ) );
    assert ( ! ( a == "73786976294838206465" ) );
    assert ( a != "73786976294838206465" );
    a = "2147483648";
    assert ( ! ( a < -2147483648 ) );
    assert ( ! ( a <= -2147483648 ) );
    assert ( a > -2147483648 );
    assert ( a >= -2147483648 );
    assert ( ! ( a == -2147483648 ) );
    assert ( a != -2147483648 );
    a = "-12345678";
    assert ( ! ( a < -87654321 ) );
    assert ( ! ( a <= -87654321 ) );
    assert ( a > -87654321 );
    assert ( a >= -87654321 );
    assert ( ! ( a == -87654321 ) );
    assert ( a != -87654321 );


  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
