#include "operators.h"
#include <iostream>
#include <variant>
#include <string>
#include <memory>
#include <cmath>
#include <deque>

//======================================================================================================================
void OperatorAdd::printTree(ostream & os) const
{
    os << '(';
    _lhs->printTree(os);
    os << '+';
    _rhs->printTree(os);
    os << ')';
}
//======================================================================================================================
void OperatorSub::printTree(ostream & os) const
{
    os << '(';
    _lhs->printTree(os);
    os << '-';
    _rhs->printTree(os);
    os << ')';
}
//======================================================================================================================
void OperatorMul::printTree(ostream & os) const
{
    _lhs->printTree(os);
    os << '*';
    _rhs->printTree(os);
}
//======================================================================================================================
void OperatorDiv::printTree(ostream & os) const
{
    _lhs->printTree(os);
    os << '/';
    _rhs->printTree(os);
}
//======================================================================================================================
void OperatorPow::printTree(ostream & os) const
{
    os << '(';
    _lhs->printTree(os);
    os << '^';
    _rhs->printTree(os);
    os << ')';
}
//======================================================================================================================
void OperatorNeg::printTree(ostream & os) const
{
    os << '(' << '-';
    _node->printTree(os);
    os << ')';
}
//======================================================================================================================
void OperatorComp::printTree(ostream & os) const
{
    os << '(';
    _lhs->printTree(os);
    os << _sign;
    _rhs->printTree(os);
    os << ')';
}
//======================================================================================================================
CValue OperatorAdd::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    CValue left = _lhs->evaluateNode(table, callers);
    CValue right = _rhs->evaluateNode(table, callers);
    CValue result;

    visit(overload
                  {
                          [&result](double l, double r) { result = l + r; },
                          [&result](const string & l, const string & r) { result = l + r; },
                          [&result](const string & l, double r) { result = l + to_string(r); },
                          [&result](double l, const string & r) { result = to_string(l) + r; },
                          [&result](const auto & l, const auto & r) { result = monostate(); }
                  }, left, right);

    return result;
}
//======================================================================================================================
CValue OperatorSub::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    CValue left = _lhs->evaluateNode(table, callers);
    CValue right = _rhs->evaluateNode(table, callers);
    CValue result;

    visit(overload
                  {
                          [&result](double l, double r) { result = l - r; },
                          [&result](const auto & l, const auto & r) { result = monostate(); }
                  }, left, right);

    return result;
}
//======================================================================================================================
CValue OperatorMul::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    CValue left = _lhs->evaluateNode(table, callers);
    CValue right = _rhs->evaluateNode(table, callers);
    CValue result;

    visit(overload
                  {
                          [&result](double l, double r) { result = l * r; },
                          [&result](const auto & l, const auto & r) { result = monostate(); }
                  }, left, right);

    return result;
}
//======================================================================================================================
CValue OperatorDiv::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    CValue left = _lhs->evaluateNode(table, callers);
    CValue right = _rhs->evaluateNode(table, callers);
    CValue result;

    visit(overload
                  {
                          [&result](double l, double r) { r == 0 ? result = monostate()
                                                                 : result = l / r; },
                          [&result](const auto & l, const auto & r) { result = monostate(); }
                  }, left, right);

    return result;
}
//======================================================================================================================
CValue OperatorPow::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    CValue left = _lhs->evaluateNode(table, callers);
    CValue right = _rhs->evaluateNode(table, callers);
    CValue result;

    visit(overload
                  {
                          [&result](double l, double r) { result = pow(l, r); },
                          [&result](const auto & l, const auto & r) { result = monostate(); }
                  }, left, right);

    return result;
}
//======================================================================================================================
CValue OperatorNeg::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    CValue value = _node->evaluateNode(table, callers);
    CValue result;

    visit(overload
                  {
                          [&result](double val) { result = -val; },
                          [&result](const auto & val) { result = monostate(); }
                  }, value);

    return result;
}
//======================================================================================================================
CValue OperatorComp::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    CValue left = _lhs->evaluateNode(table, callers);
    CValue right = _rhs->evaluateNode(table, callers);
    CValue result;

    visit(overload
                  {
                          [&](double l, double r){ compare(l, r, result); },
                          [&](const string & l, const string & r) { compare(l, r, result); },
                          [&](const auto & l, const auto & r) { result = monostate(); }
                  }, left, right);

    return result;
}
//======================================================================================================================
Node * OperatorAdd::cloneNode(int columnShift, int rowShift) const
{
    return new OperatorAdd(ANode(_lhs->cloneNode(columnShift, rowShift)),
                           ANode(_rhs->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
Node * OperatorSub::cloneNode(int columnShift, int rowShift) const
{
    return new OperatorSub(ANode(_lhs->cloneNode(columnShift, rowShift)),
                           ANode(_rhs->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
Node * OperatorMul::cloneNode(int columnShift, int rowShift) const
{
    return new OperatorMul(ANode(_lhs->cloneNode(columnShift, rowShift)),
                           ANode(_rhs->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
Node * OperatorDiv::cloneNode(int columnShift, int rowShift) const
{
    return new OperatorDiv(ANode(_lhs->cloneNode(columnShift, rowShift)),
                           ANode(_rhs->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
Node * OperatorPow::cloneNode(int columnShift, int rowShift) const
{
    return new OperatorPow(ANode(_lhs->cloneNode(columnShift, rowShift)),
                           ANode(_rhs->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
Node * OperatorNeg::cloneNode(int columnShift, int rowShift) const
{
    return new OperatorNeg(ANode(_node->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
Node * OperatorComp::cloneNode(int columnShift, int rowShift) const
{
    return new OperatorComp(ANode(_lhs->cloneNode(columnShift, rowShift)),
                            ANode(_rhs->cloneNode(columnShift, rowShift)),
                            _sign);
}
//======================================================================================================================
void OperatorComp::compare(const CValue & lhs, const CValue & rhs, CValue & result) const
{
    if (_sign == "<")
        result = (double)(lhs < rhs);
    else if (_sign == "<=")
        result = (double)(lhs <= rhs);
    else if (_sign == ">")
        result = (double)(lhs > rhs);
    else if (_sign == ">=")
        result = (double)(lhs >= rhs);
    else if (_sign == "==")
        result = (double)(lhs == rhs);
    else if (_sign == "<>")
        result = (double)(lhs != rhs);
    else
        result = monostate();
}
//======================================================================================================================
