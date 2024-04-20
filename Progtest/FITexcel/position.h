#pragma once

#include <string>

using namespace std;

//======================================================================================================================
class CPos
{
public:
    CPos() = default;
    explicit CPos(string_view str);
    explicit CPos(unsigned column, unsigned row) : _row(row), _column(column) {}
    friend bool operator<(const CPos & lhs, const CPos & rhs);

    unsigned _row = 0, _column = 0;
};
//======================================================================================================================