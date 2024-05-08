#include "operators.h"
#include <iostream>
#include <variant>
#include <string>
#include <memory>
#include <cmath>
#include <deque>

//======================================================================================================================
// Print the addition operation (+) in the expression tree.
void OperatorAdd::printTree(ostream & os) const
{
    os << '(';
    _lhs->printTree(os);
    os << '+';
    _rhs->printTree(os);
    os << ')';
}
//======================================================================================================================
// Print the subtraction operation (-) in the expression tree.
void OperatorSub::printTree(ostream & os) const
{
    os << '(';
    _lhs->printTree(os);
    os << '-';
    _rhs->printTree(os);
    os << ')';
}
//======================================================================================================================
// Print the multiplication operation (*) in the expression tree.
void OperatorMul::printTree(ostream & os) const
{
    _lhs->printTree(os);
    os << '*';
    _rhs->printTree(os);
}
//======================================================================================================================
// Print the division operation (/) in the expression tree.
void OperatorDiv::printTree(ostream & os) const
{
    _lhs->printTree(os);
    os << '/';
    _rhs->printTree(os);
}
//======================================================================================================================
// Print the power operation (^) in the expression tree.
void OperatorPow::printTree(ostream & os) const
{
    os << '(';
    _lhs->printTree(os);
    os << '^';
    _rhs->printTree(os);
    os << ')';
}
//======================================================================================================================
// Print the unary negation operation (-) in the expression tree.
void OperatorNeg::printTree(ostream & os) const
{
    os << '(' << '-';
    _node->printTree(os);
    os << ')';
}
//======================================================================================================================
// Print the comparison operation (<, <=, >, >=, ==, <>) in the expression tree.
void OperatorComp::printTree(ostream & os) const
{
    os << '(';
    _lhs->printTree(os);
    os << _sign;
    _rhs->printTree(os);
    os << ')';
}
//======================================================================================================================
// Evaluate the addition operation (+) and return the result.
CValue OperatorAdd::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    // Evaluate the left and right operands.
    CValue left = _lhs->evaluateNode(table, callers);
    CValue right = _rhs->evaluateNode(table, callers);
    CValue result;

    // Perform addition based on the type of operands.
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
// Evaluate the subtraction operation (-) and return the result.
CValue OperatorSub::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    // Evaluate the left and right operands.
    CValue left = _lhs->evaluateNode(table, callers);
    CValue right = _rhs->evaluateNode(table, callers);
    CValue result;

    // Perform subtraction based on the type of operands.
    visit(overload
                  {
                          [&result](double l, double r) { result = l - r; },
                          [&result](const auto & l, const auto & r) { result = monostate(); }
                  }, left, right);

    return result;
}
//======================================================================================================================
// Evaluate the multiplication operation (*) and return the result.
CValue OperatorMul::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    // Evaluate the left and right operands.
    CValue left = _lhs->evaluateNode(table, callers);
    CValue right = _rhs->evaluateNode(table, callers);
    CValue result;

    // Perform multiplication based on the type of operands.
    visit(overload
                  {
                          [&result](double l, double r) { result = l * r; },
                          [&result](const auto & l, const auto & r) { result = monostate(); }
                  }, left, right);

    return result;
}
//======================================================================================================================
// Evaluate the division operation (/) and return the result.
CValue OperatorDiv::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    // Evaluate the left and right operands.
    CValue left = _lhs->evaluateNode(table, callers);
    CValue right = _rhs->evaluateNode(table, callers);
    CValue result;

    // Perform division based on the type of operands.
    visit(overload
                  {
                          [&result](double l, double r) { r == 0 ? result = monostate()
                                                                        : result = l / r; },
                          [&result](const auto & l, const auto & r) { result = monostate(); }
                  }, left, right);

    return result;
}
//======================================================================================================================
// Evaluate the power operation (^) and return the result.
CValue OperatorPow::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    // Evaluate the left and right operands.
    CValue left = _lhs->evaluateNode(table, callers);
    CValue right = _rhs->evaluateNode(table, callers);
    CValue result;

    // Perform exponentiation based on the type of operands.
    visit(overload
                  {
                          [&result](double l, double r) { result = pow(l, r); },
                          [&result](const auto & l, const auto & r) { result = monostate(); }
                  }, left, right);

    return result;
}
//======================================================================================================================
// Evaluate the unary negation operation (-) and return the result.
CValue OperatorNeg::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    // Evaluate the operand.
    CValue value = _node->evaluateNode(table, callers);
    CValue result;

    // Perform unary negation based on the type of operand.
    visit(overload
                  {
                          [&result](double val) { result = -val; },
                          [&result](const auto & val) { result = monostate(); }
                  }, value);

    return result;
}
//======================================================================================================================
// Evaluate the comparison operation and return the result.
CValue OperatorComp::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    // Evaluate the left and right operands.
    CValue left = _lhs->evaluateNode(table, callers);
    CValue right = _rhs->evaluateNode(table, callers);
    CValue result;

    // Perform comparison based on the comparison sign.
    visit(overload
                  {
                          [&](double l, double r){ compare(l, r, result); },
                          [&](const string & l, const string & r) { compare(l, r, result); },
                          [&](const auto & l, const auto & r) { result = monostate(); }
                  }, left, right);

    return result;
}
//======================================================================================================================
// Clone the addition operator node with adjusted column and row shifts.
Node * OperatorAdd::cloneNode(int columnShift, int rowShift) const
{
    return new OperatorAdd(ANode(_lhs->cloneNode(columnShift, rowShift)),
                           ANode(_rhs->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
// Clone the subtraction operator node with adjusted column and row shifts.
Node * OperatorSub::cloneNode(int columnShift, int rowShift) const
{
    return new OperatorSub(ANode(_lhs->cloneNode(columnShift, rowShift)),
                           ANode(_rhs->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
// Clone the multiplication operator node with adjusted column and row shifts.
Node * OperatorMul::cloneNode(int columnShift, int rowShift) const
{
    return new OperatorMul(ANode(_lhs->cloneNode(columnShift, rowShift)),
                           ANode(_rhs->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
// Clone the division operator node with adjusted column and row shifts.
Node * OperatorDiv::cloneNode(int columnShift, int rowShift) const
{
    return new OperatorDiv(ANode(_lhs->cloneNode(columnShift, rowShift)),
                           ANode(_rhs->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
// Clone the power operator node with adjusted column and row shifts.
Node * OperatorPow::cloneNode(int columnShift, int rowShift) const
{
    return new OperatorPow(ANode(_lhs->cloneNode(columnShift, rowShift)),
                           ANode(_rhs->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
// Clone the unary negation operator node with adjusted column and row shifts.
Node * OperatorNeg::cloneNode(int columnShift, int rowShift) const
{
    return new OperatorNeg(ANode(_node->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
// Clone the comparison operator node with adjusted column and row shifts.
Node * OperatorComp::cloneNode(int columnShift, int rowShift) const
{
    return new OperatorComp(ANode(_lhs->cloneNode(columnShift, rowShift)),
                            ANode(_rhs->cloneNode(columnShift, rowShift)),
                            _sign);
}
//======================================================================================================================
// Helper function to perform actual comparison based on the comparison sign.
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
        result = monostate(); // Invalid comparison sign.
}
//======================================================================================================================