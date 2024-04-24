#include "functions.h"
#include <iostream>
#include <variant>
#include <string>
#include <memory>
#include <cmath>
#include <deque>
#include <cfloat>

//======================================================================================================================
void FunctionSum::printTree(ostream & os) const
{
    os << "sum(";
    _range->printTree(os);
    os << ")";
}
//======================================================================================================================
void FunctionCount::printTree(ostream & os) const
{
    os << "count(";
    _range->printTree(os);
    os << ")";
}
//======================================================================================================================
void FunctionMin::printTree(ostream & os) const
{
    os << "min(";
    _range->printTree(os);
    os << ")";
}
//======================================================================================================================
void FunctionMax::printTree(ostream & os) const
{
    os << "max(";
    _range->printTree(os);
    os << ")";
}
//======================================================================================================================
void FunctionCountVal::printTree(ostream & os) const
{
    os << "countval(";
    _value->printTree(os);
    os << ", ";
    _range->printTree(os);
    os << ")";
}
//======================================================================================================================
void FunctionIf::printTree(ostream & os) const
{
    os << "if(";
    _condition->printTree(os);
    os << ", ";
    _ifTrue->printTree(os);
    os << ", ";
    _ifFalse->printTree(os);
    os << ")";
}
//======================================================================================================================
CValue FunctionSum::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    double result = 0.0;
    bool foundNumber = false;
    auto range = _range->cloneNode(0, 0);
    auto copyRange = dynamic_cast<RangeOperand*>(range);
    for (auto column = copyRange->_upperLeft._column; column <= copyRange->_lowerRight._column; column++)
    {
        for (auto row = copyRange->_upperLeft._row; row <= copyRange->_lowerRight._row; row++)
        {
            auto position = table.find(CPos(column, row));
            if (position == table.end())
                continue;
            CValue tmp = position->second->evaluateNode(table, callers);
            visit(overload
            {
                [&result, &foundNumber](double value)
                {
                    result += value;
                    foundNumber = true;
                },
                [](const auto & value) {  }
            }, tmp);
        }
    }
    delete range;
    if (foundNumber)
        return {result};
    else
        return {};
}
//======================================================================================================================
CValue FunctionCount::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    double count = 0.0;
    auto range = _range->cloneNode(0, 0);
    auto copyRange = dynamic_cast<RangeOperand*>(range);
    for (auto column = copyRange->_upperLeft._column; column <= copyRange->_lowerRight._column; column++)
    {
        for (auto row = copyRange->_upperLeft._row; row <= copyRange->_lowerRight._row; row++)
        {
            auto position = table.find(CPos(column, row));
            if (position == table.end())
                continue;
            CValue tmp = position->second->evaluateNode(table, callers);
            visit(overload
            {
                [&count](double value) { count++; },
                [&count](const string & value) { count++; },
                [](const auto & value) {  }
            }, tmp);
        }
    }
    delete range;
    copyRange = nullptr;
    range = nullptr;
    return {count};
}
//======================================================================================================================
CValue FunctionMin::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    auto min = DBL_MAX;
    bool foundNumber = false;
    auto range = _range->cloneNode(0, 0);
    auto copyRange = dynamic_cast<RangeOperand*>(range);
    for (auto column = copyRange->_upperLeft._column; column <= copyRange->_lowerRight._column; column++)
    {
        for (auto row = copyRange->_upperLeft._row; row <= copyRange->_lowerRight._row; row++)
        {
            auto position = table.find(CPos(column, row));
            if (position == table.end())
                continue;
            CValue tmp = position->second->evaluateNode(table, callers);
            visit(overload
            {
                [&min, &foundNumber](double value)
                {
                    min = (value < min) ? value : min;
                    foundNumber = true;
                },
                [](const auto & value) {  }
            }, tmp);
        }
    }
    delete range;
    if (foundNumber)
        return {min};
    else
        return {};
}
//======================================================================================================================
CValue FunctionMax::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    auto max = DBL_MIN;
    bool foundNumber = false;
    auto range = _range->cloneNode(0, 0);
    auto copyRange = dynamic_cast<RangeOperand*>(range);
    for (auto column = copyRange->_upperLeft._column; column <= copyRange->_lowerRight._column; column++)
    {
        for (auto row = copyRange->_upperLeft._row; row <= copyRange->_lowerRight._row; row++)
        {
            auto position = table.find(CPos(column, row));
            if (position == table.end())
                continue;
            CValue tmp = position->second->evaluateNode(table, callers);
            visit(overload
            {
                [&max, &foundNumber](double value)
                {
                    max = (value > max) ? value : max;
                    foundNumber = true;
                },
                [](const auto & value) {  }
            }, tmp);
        }
    }
    delete range;
    if (foundNumber)
        return {max};
    else
        return {};
}
//======================================================================================================================
CValue FunctionCountVal::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    double count = 0.0;
    CValue valueToCount = _value->evaluateNode(table, callers);
    auto range = _range->cloneNode(0, 0);
    auto copyRange = dynamic_cast<RangeOperand*>(range);
    for (auto column = copyRange->_upperLeft._column; column <= copyRange->_lowerRight._column; column++)
    {
        for (auto row = copyRange->_upperLeft._row; row <= copyRange->_lowerRight._row; row++)
        {
            auto position = table.find(CPos(column, row));
            if (position == table.end())
                continue;
            CValue tmp = position->second->evaluateNode(table, callers);
            visit(overload
            {
                [&count] (double val1, double val2){ count = (val1 == val2) ? count + 1 : count; },
                [&count] (const string & val1, const string & val2) { count = (val1 == val2) ? count + 1
                                                                                                    : count; },
                [] (const auto & val1 , const auto & val2) {  }
            }, tmp, valueToCount);
        }
    }
    delete range;
    return {count};
}
//======================================================================================================================
CValue FunctionIf::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    CValue condition = _condition->evaluateNode(table, callers);
    CValue result;
    visit(overload
    {
        [&](double value)
        {
            if (value != 0.0)
                result = _ifTrue->evaluateNode(table, callers);
            else
                result = _ifFalse->evaluateNode(table, callers);
        },
        [&result] (const auto & value) { result = monostate(); }
    }, condition);
    return result;
}
//======================================================================================================================
Node * FunctionSum::cloneNode(int columnShift, int rowShift) const
{
    return new FunctionSum(ANode(_range->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
Node * FunctionCount::cloneNode(int columnShift, int rowShift) const
{
    return new FunctionCount(ANode(_range->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
Node * FunctionMin::cloneNode(int columnShift, int rowShift) const
{
    return new FunctionMin(ANode(_range->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
Node * FunctionMax::cloneNode(int columnShift, int rowShift) const
{
    return new FunctionMax(ANode(_range->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
Node * FunctionCountVal::cloneNode(int columnShift, int rowShift) const
{
    return new FunctionCountVal(ANode(_value->cloneNode(columnShift, rowShift)),
                                ANode(_range->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
Node * FunctionIf::cloneNode(int columnShift, int rowShift) const
{
    return new FunctionIf(ANode(_condition->cloneNode(columnShift, rowShift)),
                          ANode(_ifTrue->cloneNode(columnShift, rowShift)),
                          ANode(_ifFalse->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================