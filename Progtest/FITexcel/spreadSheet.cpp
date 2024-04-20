#include "spreadSheet.h"
#include <iostream>

//======================================================================================================================
CSpreadsheet::CSpreadsheet(const CSpreadsheet & other)
{
    _table.clear();
    for (const auto & element : other._table)
        _table.emplace(element.first, element.second->cloneNode());
    _stringTable.clear();
    for (const auto & element : other._stringTable)
        _stringTable[element.first] = element.second;
}
//======================================================================================================================
CSpreadsheet::CSpreadsheet(CSpreadsheet && other) noexcept
{
    _table = std::move(other._table);
    _stringTable = std::move(other._stringTable);
}
//======================================================================================================================
CSpreadsheet & CSpreadsheet::operator=(const CSpreadsheet & other)
{
    if (this == &other)
        return *this;

    CSpreadsheet copyToSwap(other);
    swap(_table, copyToSwap._table);
    swap(_stringTable, copyToSwap._stringTable);
    return *this;
}
//======================================================================================================================
CSpreadsheet & CSpreadsheet::operator=(CSpreadsheet && other) noexcept
{
    if (this == &other)
        return *this;

    _table = std::move(other._table);
    _stringTable = std::move(other._stringTable);
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
        if (cell[index] < '0' || cell[index] > '9')
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
        spreadSheet.setCell(CPos(stoul(column), stoul(row)), value);
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
    _stringTable = std::move(newSpreadSheet._stringTable);
    return true;
}
//======================================================================================================================
bool CSpreadsheet::save(std::ostream & os) const
{
    string fullTable;
    for (const auto & element : _stringTable)
        fullTable += "c" + to_string(element.first._column) +
                     "r" + to_string(element.first._row) +
                     "_" + element.second + '|';
    if (!fullTable.empty())
        fullTable.erase(fullTable.size() - 1);
    else
        return false;

    os << fullTable;
    return true;
}
//======================================================================================================================
bool CSpreadsheet::setCell(CPos pos, std::string contents)
{
    try
    {
        Builder builder;
        parseExpression(contents, builder);
        _table[pos] = std::move(builder.builderStack.top());
        _stringTable[pos] = std::move(contents);
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
//void copyRect(CPos dst, CPos src, int w = 1, int h = 1) {} // todo
//======================================================================================================================