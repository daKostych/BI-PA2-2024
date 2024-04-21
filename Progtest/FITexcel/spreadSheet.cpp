#include "spreadSheet.h"
#include <iostream>
#include <cmath>

//======================================================================================================================
CSpreadsheet::CSpreadsheet(const CSpreadsheet & other)
{
    _table.clear();
    for (const auto & element : other._table)
        _table.emplace(element.first, element.second->cloneNode(0, 0));
}
//======================================================================================================================
CSpreadsheet::CSpreadsheet(CSpreadsheet && other) noexcept
{
    _table = std::move(other._table);
}
//======================================================================================================================
CSpreadsheet & CSpreadsheet::operator=(const CSpreadsheet & other)
{
    if (this == &other)
        return *this;

    CSpreadsheet copyToSwap(other);
    swap(_table, copyToSwap._table);
    return *this;
}
//======================================================================================================================
CSpreadsheet & CSpreadsheet::operator=(CSpreadsheet && other) noexcept
{
    if (this == &other)
        return *this;

    _table = std::move(other._table);
    return *this;
}
//======================================================================================================================
bool CSpreadsheet::loadSell(const string & cell, CSpreadsheet & spreadSheet)
{
    string column, row ,value;
    if (cell[0] != 'c')
        return false;

    size_t index = 1;
    for (; index < cell.size() && cell[index] != 'r'; index++)
    {
        if ((cell[index] < 'a' || cell[index] > 'z') &&
            (cell[index] < 'A' || cell[index] > 'Z'))
            return false;
        column += cell[index];
    }
    index++;
    for (; index < cell.size() && cell[index] != '_'; index++)
    {
        if (cell[index] < '0' || cell[index] > '9')
            return false;
        row += cell[index];
    }
    index++;
    for (; index < cell.size(); index++)
        value += cell[index];

    try
    {
        spreadSheet.setCell(CPos(column + row), value);
        return true;
    }
    catch (const exception & ex ) { return false; }
}
//======================================================================================================================
bool CSpreadsheet::load(std::istream & is)
{
    CSpreadsheet newSpreadSheet;
    string fullTable, tablePart;
    while (getline(is, tablePart))
        fullTable += tablePart;
    for(size_t i = 0, from = 0; i < fullTable.size(); i++)
    {
        if (fullTable[i] == '|')
        {
            if (!loadSell(fullTable.substr(from, i - from), newSpreadSheet))
                return false;
            from = i + 1;
        }
    }
    _table = std::move(newSpreadSheet._table);
    return true;
}
//======================================================================================================================
bool CSpreadsheet::save(std::ostream & os) const
{
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
        parseExpression(std::move(contents), builder);
        _table[pos] = std::move(builder.builderStack.top());
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
        return std::monostate();
    return _table[pos]->evaluateNode(_table);
}
//======================================================================================================================
void CSpreadsheet::copyRect(CPos dst, CPos src, int w, int h)
{
    if (w < 1 || h < 1 || dst == src)
        return;

    map<CPos, ANode> tmpTable;
    map<CPos, string> tmpStringTable;
    int columnShift = static_cast<int>(max(dst._column, src._column) - min(dst._column, src._column));
    if (max(dst._column, src._column) == src._column)
        columnShift = -columnShift;
    int rowShift = static_cast<int>(max(dst._row, src._row) - min(dst._row, src._row));
    if (max(dst._row, src._row) == src._row)
        rowShift = -rowShift;

    for (size_t column = src._column; column < src._column + w; column++)
    {
        for (size_t row = src._row; row < src._row + h; row++)
        {
            auto iterator = _table.find(CPos(column, row));
            if (iterator != _table.end())
                tmpTable.emplace(CPos(column + columnShift, row + rowShift),
                                 _table[CPos(column, row)]->cloneNode(columnShift, rowShift));
            else
                tmpTable.emplace(CPos(column + columnShift, row + rowShift),
                                 new Operand(monostate()));
        }
    }

    for (auto & element : tmpTable)
        _table[element.first] = std::move(element.second);
}
//======================================================================================================================