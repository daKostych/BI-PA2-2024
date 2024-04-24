#include "node.h"
#include <variant>
#include <string>
#include <memory>
#include <cmath>
#include <deque>

using CValue = std::variant<std::monostate, double, std::string>;
using ANode = unique_ptr<Node>;
//======================================================================================================================
void Operand::printTree(ostream & os) const
{
    visit(overload
    {
        [&os](double val) { os << val; },
        [&os](const string & val)
        {
            os << "\"";
            for (char c : val)
            {
                if (c == '"')
                    os << "\"\"";
                else
                    os << c;
            }
            os << "\"";
        },
        [](const auto & val) {}
    }, _val);
}
//======================================================================================================================
void RefOperand::printTree(ostream & os) const
{
    if (_columnAbsolute)
        os << '$';
    os << _referencedPosition.columnToString();
    if (_rowAbsolute)
        os << '$';
    os << _referencedPosition._row;
}
//======================================================================================================================
void RangeOperand::printTree(ostream & os) const
{
    if (_ulColumnAbs)
        os << '$';
    os << _upperLeft.columnToString();
    if (_ulRowAbs)
        os << '$';
    os << _upperLeft._row;
    os << ":";
    if (_lrColumnAbs)
        os << '$';
    os << _lowerRight.columnToString();
    if (_lrRowAbs)
        os << '$';
    os << _lowerRight._row;
}
//======================================================================================================================
CValue Operand::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const { return _val; }
//======================================================================================================================
CValue RefOperand::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    auto iterator = table.find(_referencedPosition);
    if (iterator == table.end())
        return monostate();

    auto callerPos = find(callers.begin(), callers.end(), _referencedPosition);
    if (callerPos != callers.end())
        return monostate();
    callers.push_back(_referencedPosition);
    CValue result = iterator->second->evaluateNode(table, callers);
    callers.pop_back();
    return result;
}
//======================================================================================================================
CValue RangeOperand::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const { return monostate(); }
//======================================================================================================================
Node * Operand::cloneNode(int columnShift, int rowShift) const { return new Operand(_val); }
//======================================================================================================================
Node * RefOperand::cloneNode(int columnShift, int rowShift) const
{
    ShiftedPos shifted = shiftPosition(_referencedPosition,
                                       columnShift, rowShift,
                                       _rowAbsolute, _columnAbsolute);

    return new RefOperand(CPos(shifted.first, shifted.second),
                          _rowAbsolute,
                          _columnAbsolute);
}
//======================================================================================================================
Node * RangeOperand::cloneNode(int columnShift, int rowShift) const
{
    ShiftedPos shiftedUpperLeft = shiftPosition(_upperLeft,
                                                columnShift, rowShift,
                                                _ulRowAbs, _ulColumnAbs);

    ShiftedPos shiftedLowerRight = shiftPosition(_lowerRight,
                                                 columnShift, rowShift,
                                                 _lrRowAbs, _lrColumnAbs);

    return new RangeOperand(CPos(shiftedUpperLeft.first, shiftedUpperLeft.second),
                            CPos(shiftedLowerRight.first, shiftedLowerRight.second),
                            _ulColumnAbs, _ulRowAbs,
                            _lrColumnAbs, _lrRowAbs);
}
//======================================================================================================================
ShiftedPos Node::shiftPosition(const CPos & pos, int columnShift, int rowShift, bool rowAbs, bool columnAbs)
{
    unsigned newRow = pos._row,
            newColumn = pos._column;
    if (!rowAbs)
        newRow += rowShift;
    if (!columnAbs)
        newColumn += columnShift;
    return make_pair(newColumn, newRow);
}
