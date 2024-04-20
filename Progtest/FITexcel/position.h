#pragma once

#include <string>

using namespace std;

//======================================================================================================================
class CPos
{
public:
    CPos() = default;
    explicit CPos(string_view str);
    friend bool operator<(const CPos & lhs, const CPos & rhs);

    unsigned _row = 0, _column = 0;
};
//======================================================================================================================