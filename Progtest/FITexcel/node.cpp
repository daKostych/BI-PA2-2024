#include "node.h"
#include <variant>
#include <string>
#include <memory>
#include <cmath>
#include <deque>

using CValue = variant<monostate, double, string>;
using ANode = unique_ptr<Node>;
//======================================================================================================================
// Print the value of an Operand node to the output stream.
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
// Print the referenced position of a RefOperand node to the output stream.
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
// Print the range boundaries of a RangeOperand node to the output stream.
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
// Evaluate the value of an Operand node.
CValue Operand::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const { return _val; }
//======================================================================================================================
// Evaluate the value referenced by a RefOperand node from a given table of values.
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
// Evaluate the value of a RangeOperand node.
CValue RangeOperand::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const { return monostate(); }
//======================================================================================================================
// Clone an Operand node.
Node * Operand::cloneNode(int columnShift, int rowShift) const { return new Operand(_val); }
//======================================================================================================================
// Clone a RefOperand node with optional column and row shifts.
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
// Clone a RangeOperand node with optional column and row shifts.
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
// Utility function to shift a position by specified column and row shifts based on absolute flags.
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