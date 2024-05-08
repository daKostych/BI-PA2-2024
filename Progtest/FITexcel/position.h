#pragma once

#include <string>

using namespace std;

//======================================================================================================================
class CPos
{
public:
    // Constructors
    CPos() = default;
    explicit CPos(string_view str);
    explicit CPos(unsigned column, unsigned row) : _row(row), _column(column) {}

    // Member function to convert column to string representation
    string columnToString() const;

    // Friend function
    friend bool isValidRange(const CPos & upperLeft, const CPos & lowerRight);

    // Friend operators
    friend bool operator<(const CPos & lhs, const CPos & rhs);
    friend bool operator==(const CPos & lhs, const CPos & rhs);

    unsigned _row = 0, _column = 0;  // Member variables representing row and column
};
//======================================================================================================================
bool isValidRange(const CPos & upperLeft, const CPos & lowerRight);