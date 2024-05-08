#include "position.h"
#include <string>
#include <stdexcept>

//======================================================================================================================
CPos::CPos(string_view str)
{
    string index(str);

    // Find the first non-alphanumeric character
    auto position = find_if(index.begin(), index.end(), [](char & symbol)
    {
        return !isalnum(symbol);
    });

    // If non-alphanumeric character is found, throw an exception
    if (position != index.end())
        throw invalid_argument("Invalid position.");

    // Find the index where the column string ends (first digit encountered)
    size_t columnEnd = index.find_first_of("1234567890");
    unsigned calculateColumn = 0;

    // Calculate the column number
    for (size_t i = 0; i < columnEnd; i++)
    {
        char symbol = (char)toupper(index[i]);
        if (symbol < 'A' || symbol > 'Z')
            throw invalid_argument("Invalid position.");
        calculateColumn *= 26;
        calculateColumn += symbol - 'A' + 1;
    }

    // Set the column and row based on the parsed values from the input string
    _column = calculateColumn;
    _row = stoi(index.substr(columnEnd)); // Convert the remaining string (after column) to integer for row
}
//======================================================================================================================
string CPos::columnToString() const
{
    string result;
    int column = static_cast<int>(_column);

    // Convert column number to its string representation
    for (; column > 0; column = (column - 1) / 26)
        result.insert(result.begin(), (char)((column - 1) % 26 + 'A'));
    return result;
}
//======================================================================================================================
// Check if the range is valid
bool isValidRange(const CPos & upperLeft, const CPos & lowerRight)
{
    return (upperLeft._column <= lowerRight._column) &&
           (upperLeft._row <= lowerRight._row);
}
//======================================================================================================================
bool operator<(const CPos & lhs, const CPos & rhs)
{
    // Compare positions based on column first, then row if columns are equal
    if (lhs._column != rhs._column)
        return lhs._column < rhs._column;
    return lhs._row < rhs._row;
}
//======================================================================================================================
bool operator==(const CPos & lhs, const CPos & rhs)
{
    // Check if two positions are identical (same row and column)
    return lhs._row == rhs._row &&
           lhs._column == rhs._column;
}
//======================================================================================================================