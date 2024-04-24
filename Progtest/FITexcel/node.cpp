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
CValue RangeOperand::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const { return monostate(); } // todo
//======================================================================================================================
Node * Operand::cloneNode(int columnShift, int rowShift) const { return new Operand(_val); }
//======================================================================================================================
Node * RefOperand::cloneNode(int columnShift, int rowShift) const
{
    unsigned newRow = _referencedPosition._row,
             newColumn = _referencedPosition._column;
    if (!_rowAbsolute)
        newRow += rowShift;
    if (!_columnAbsolute)
        newColumn += columnShift;

    return new RefOperand(CPos(newColumn, newRow),
                          _rowAbsolute,
                          _columnAbsolute);
}
//======================================================================================================================
Node * RangeOperand::cloneNode(int columnShift, int rowShift) const
{
    unsigned newUpperLeftRow = _upperLeft._row,
             newUpperLeftColumn = _upperLeft._column;
    if (!_ulRowAbs)
        newUpperLeftRow += rowShift;
    if (!_ulColumnAbs)
        newUpperLeftColumn += columnShift;

    unsigned newLowerRightRow = _lowerRight._row,
             newLowerRightColumn = _lowerRight._column;
    if (!_lrRowAbs)
        newLowerRightRow += rowShift;
    if (!_lrColumnAbs)
        newLowerRightColumn += columnShift;

    return new RangeOperand(CPos(newUpperLeftColumn, newUpperLeftRow),
                            CPos(newLowerRightColumn, newLowerRightRow),
                            _ulColumnAbs, _ulRowAbs,
                            _lrColumnAbs, _lrRowAbs);
}
//======================================================================================================================