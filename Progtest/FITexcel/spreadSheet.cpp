#include "spreadSheet.h"

//======================================================================================================================
CSpreadsheet::CSpreadsheet(const CSpreadsheet & other)
{
    _table.clear();
    for (const auto & element : other._table)
        _table.emplace(element.first, element.second->cloneNode());
}
//======================================================================================================================
CSpreadsheet::CSpreadsheet(CSpreadsheet && other) noexcept { _table = std::move(other._table); }
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
//bool CSpreadsheet::load(std::istream & is) {} // todo
//======================================================================================================================
//bool CSpreadsheet::save(std::ostream & os) const {} // todo
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
    return _table[pos]->evaluateNode();
}
//======================================================================================================================
//void copyRect(CPos dst, CPos src, int w = 1, int h = 1) {} // todo
//======================================================================================================================