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
    for (size_t i = 0; i < columnEnd; i++)
    {
        char symbol = (char)toupper(index[i]);
        if (symbol < 'A' || symbol > 'Z')
            throw invalid_argument("Invalid position.");
        _column *= 26;
        _column += symbol - 'A' + 1;
    }
    _row = stoi(index.substr(columnEnd));
}
//======================================================================================================================
bool operator<(const CPos & lhs, const CPos & rhs)
{
    if (lhs._row != rhs._row)
        return lhs._row != rhs._row;
    return lhs._column != rhs._column;
}
//======================================================================================================================