#include "spreadSheet.h"
#include <iostream>
#include <cmath>

//======================================================================================================================
CSpreadsheet::CSpreadsheet(const CSpreadsheet & other)
{
    _table.clear(); // Clear the current table
    // Iterate over each element in the other table and clone each node, then add to _table
    for (const auto & element : other._table)
        _table.emplace(element.first, element.second->cloneNode(0, 0));
}
//======================================================================================================================
CSpreadsheet::CSpreadsheet(CSpreadsheet && other) noexcept
{
    _table = std::move(other._table); // Move other's table to this object
}
//======================================================================================================================
CSpreadsheet & CSpreadsheet::operator=(const CSpreadsheet & other)
{
    if (this == &other)
        return *this;

    // Create a temporary copy of other using copy constructor
    CSpreadsheet copyToSwap(other);
    // Swap the content of _table with the temporary table
    swap(_table, copyToSwap._table);
    return *this;
}
//======================================================================================================================
CSpreadsheet & CSpreadsheet::operator=(CSpreadsheet && other) noexcept
{
    if (this == &other)
        return *this;

    _table = std::move(other._table); // Move other's table to this object
    return *this;
}
//======================================================================================================================
bool CSpreadsheet::loadSell(const string & cell, CSpreadsheet & spreadSheet)
{
    string column, row, value;
    if (cell[0] != 'c')
        return false;

    size_t index = 1;

    // Extract column from the cell string
    for (; index < cell.size() && cell[index] != 'r'; index++)
    {
        if (cell[index] < 'A' || cell[index] > 'Z')
            return false;
        column += cell[index];
    }
    index++;

    // Extract row from the cell string
    for (; index < cell.size() && cell[index] != '_'; index++)
    {
        if (cell[index] < '0' || cell[index] > '9')
            return false;
        row += cell[index];
    }
    index++;

    // Extract value from the cell string
    value = cell.substr(index);

    try
    {
        // Set the cell contents in the provided spreadsheet object
        spreadSheet.setCell(CPos(column + row), value);
        return true;
    }
    catch (const exception & ex ) { return false; }
}
//======================================================================================================================
bool CSpreadsheet::load(std::istream & is)
{
    CSpreadsheet newSpreadSheet; // Create a new temporary spreadsheet
    string fullTable, tablePart;

    // Read the entire table from the input stream
    while (getline(is, tablePart))
        fullTable += tablePart + '\n';
    fullTable.erase(fullTable.size()-1);

    // Parse the full table and load into the temporary spreadsheet
    for(size_t i = 0, from = 0; i < fullTable.size(); i++)
    {
        if (fullTable[i] == '|')
        {
            // Load each cell defined by the substring between '|' characters
            if (!loadSell(fullTable.substr(from, i - from), newSpreadSheet))
                return false;
            from = i + 1;
        }
    }
    // Move the table from the temporary spreadsheet to this object
    _table = std::move(newSpreadSheet._table);
    return true;
}
//======================================================================================================================
bool CSpreadsheet::save(std::ostream & os) const
{
    // Iterate over each element in _table and save to output stream
    for (const auto & element : _table)
    {
        os << "c" + element.first.columnToString() +
              "r" + to_string(element.first._row) +
              "_=";
        element.second->printTree(os);
        os << '|';
    }
    return true;
}
//======================================================================================================================
bool CSpreadsheet::setCell(CPos pos, std::string contents)
{
    try
    {
        Builder builder;
        parseExpression(std::move(contents), builder);  // Parse the contents using function parseExpression
        _table[pos] = std::move(builder.builderStack.top());   // Set the cell value
    }
    catch (const exception & ex)
    {
        return false;
    }
    return true;
}
//======================================================================================================================
CValue CSpreadsheet::getValue(CPos pos)
{
    auto iterator = _table.find(pos);
    if (iterator == _table.end())
        return monostate(); // Return monostate if cell is not found
    deque<CPos> callers;
    callers.push_back(pos);
    // Evaluate the node stored in the cell and return the value
    return _table[pos]->evaluateNode(_table, callers);
}
//======================================================================================================================
void CSpreadsheet::copyRect(CPos dst, CPos src, int w, int h)
{
    if (w < 1 || h < 1 || dst == src)
        return;

    map<CPos, ANode> tmpTable;
    int columnShift = static_cast<int>(max(dst._column, src._column) - min(dst._column, src._column));
    if (max(dst._column, src._column) == src._column)
        columnShift = -columnShift;
    int rowShift = static_cast<int>(max(dst._row, src._row) - min(dst._row, src._row));
    if (max(dst._row, src._row) == src._row)
        rowShift = -rowShift;

    // Iterate over the specified rectangular area and copy cells with adjusted positions
    for (size_t column = src._column; column < src._column + w; column++)
    {
        for (size_t row = src._row; row < src._row + h; row++)
        {
            auto iterator = _table.find(CPos(column, row));
            if (iterator != _table.end())
                tmpTable.emplace(CPos(column + columnShift, row + rowShift),
                                 _table[CPos(column, row)]->cloneNode(columnShift, rowShift));
        }
    }

    // Move the copied cells back to the original table
    for (auto & element : tmpTable)
        _table[element.first] = std::move(element.second);
}
//======================================================================================================================