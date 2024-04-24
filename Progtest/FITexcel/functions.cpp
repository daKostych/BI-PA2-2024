#include "functions.h"
#include <iostream>
#include <variant>
#include <string>
#include <memory>
#include <cmath>
#include <deque>

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
CValue FunctionSum::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const {} // todo
//======================================================================================================================
CValue FunctionCount::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const {} // todo
//======================================================================================================================
CValue FunctionMin::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const {} // todo
//======================================================================================================================
CValue FunctionMax::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const {} // todo
//======================================================================================================================
CValue FunctionCountVal::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const {} // todo
//======================================================================================================================
CValue FunctionIf::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const {} // todo
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