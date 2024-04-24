#include "position.h"
#include <string>
#include <stdexcept>

//======================================================================================================================
CPos::CPos(string_view str)
{
    string index(str);
    auto position = find_if(index.begin(), index.end(), [](char & symbol)
    {
        return !isalnum(symbol);
    });
    if (position != index.end())
        throw invalid_argument("Invalid position.");

    size_t columnEnd = index.find_first_of("1234567890");
    unsigned calculateColumn = 0;
    for (size_t i = 0; i < columnEnd; i++)
    {
        char symbol = (char)toupper(index[i]);
        if (symbol < 'A' || symbol > 'Z')
            throw invalid_argument("Invalid position.");
        calculateColumn *= 26;
        calculateColumn += symbol - 'A' + 1;
    }
    _column = calculateColumn;
    _row = stoi(index.substr(columnEnd));
}
//======================================================================================================================
string CPos::columnToString() const
{
    string result;
    int column = static_cast<int>(_column);
    for (; column > 0; column = (column - 1) / 26)
        result.insert(result.begin(), (char)((column - 1) % 26 + 'A'));
    return result;
}
//======================================================================================================================
bool isValidRange(const CPos & upperLeft, const CPos & lowerRight)
{
    return (upperLeft._column <= lowerRight._column) &&
           (upperLeft._row <= lowerRight._row);
}
//======================================================================================================================
bool operator<(const CPos & lhs, const CPos & rhs)
{
    if (lhs._column != rhs._column)
        return lhs._column < rhs._column;
    return lhs._row < rhs._row;
}
//======================================================================================================================
bool operator==(const CPos & lhs, const CPos & rhs)
{
    return lhs._row == rhs._row &&
           lhs._column == rhs._column;
}
//======================================================================================================================