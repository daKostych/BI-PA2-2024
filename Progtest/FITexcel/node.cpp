#include "node.h"
#include <variant>
#include <string>
#include <memory>
#include <cmath>

//https://www.cppstories.com/2018/06/variant/
template<class... Ts>
struct overload : Ts ...{ using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

using CValue = std::variant<std::monostate, double, std::string>;
using ANode = unique_ptr<Node>;
//======================================================================================================================
CValue Operand::evaluateNode(map<CPos, ANode> & table) const { return _val; }
//======================================================================================================================
CValue RefOperand::evaluateNode(map<CPos, ANode> & table) const
{
    return table[_referencedPosition]->evaluateNode(table);
}
//======================================================================================================================
CValue OperatorAdd::evaluateNode(map<CPos, ANode> & table) const
{
    CValue left = _lhs->evaluateNode(table);
    CValue right = _rhs->evaluateNode(table);
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
CValue OperatorSub::evaluateNode(map<CPos, ANode> & table) const
{
    CValue left = _lhs->evaluateNode(table);
    CValue right = _rhs->evaluateNode(table);
    CValue result;

    visit(overload
    {
        [&result](double l, double r) { result = l - r; },
        [&result](const auto & l, const auto & r) { result = monostate(); }
    }, left, right);

    return result;
}
//======================================================================================================================
CValue OperatorMul::evaluateNode(map<CPos, ANode> & table) const
{
    CValue left = _lhs->evaluateNode(table);
    CValue right = _rhs->evaluateNode(table);
    CValue result;

    visit(overload
    {
        [&result](double l, double r) { result = l * r; },
        [&result](const auto & l, const auto & r) { result = monostate(); }
    }, left, right);

    return result;
}
//======================================================================================================================
CValue OperatorDiv::evaluateNode(map<CPos, ANode> & table) const
{
    CValue left = _lhs->evaluateNode(table);
    CValue right = _rhs->evaluateNode(table);
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
CValue OperatorPow::evaluateNode(map<CPos, ANode> & table) const
{
    CValue left = _lhs->evaluateNode(table);
    CValue right = _rhs->evaluateNode(table);
    CValue result;

    visit(overload
    {
        [&result](double l, double r) { result = pow(l, r); },
        [&result](const auto & l, const auto & r) { result = monostate(); }
    }, left, right);

    return result;
}
//======================================================================================================================
CValue OperatorNeg::evaluateNode(map<CPos, ANode> & table) const
{
    CValue value = _node->evaluateNode(table);
    CValue result;

    visit(overload
    {
        [&result](double val) { result = -val; },
        [&result](const auto & val) { result = monostate(); }
    }, value);

    return result;
}
//======================================================================================================================
CValue OperatorComp::evaluateNode(map<CPos, ANode> & table) const
{
    CValue left = _lhs->evaluateNode(table);
    CValue right = _rhs->evaluateNode(table);
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
Node * Operand::cloneNode() const { return new Operand(_val); }
//======================================================================================================================
Node * RefOperand::cloneNode() const { return new RefOperand(_referencedPosition,
                                                             _rowAbsolute,
                                                             _columnAbsolute); }
//======================================================================================================================
Node * OperatorAdd::cloneNode() const
{
    return new OperatorAdd(ANode(_lhs->cloneNode()), ANode(_rhs->cloneNode()));
}
//======================================================================================================================
Node * OperatorSub::cloneNode() const
{
    return new OperatorSub(ANode(_lhs->cloneNode()), ANode(_rhs->cloneNode()));
}
//======================================================================================================================
Node * OperatorMul::cloneNode() const
{
    return new OperatorMul(ANode(_lhs->cloneNode()), ANode(_rhs->cloneNode()));
}
//======================================================================================================================
Node * OperatorDiv::cloneNode() const
{
    return new OperatorDiv(ANode(_lhs->cloneNode()), ANode(_rhs->cloneNode()));
}
//======================================================================================================================
Node * OperatorPow::cloneNode() const
{
    return new OperatorPow(ANode(_lhs->cloneNode()), ANode(_rhs->cloneNode()));
}
//======================================================================================================================
Node * OperatorNeg::cloneNode() const
{
    return new OperatorNeg(ANode(_node->cloneNode()));
}
//======================================================================================================================
Node * OperatorComp::cloneNode() const
{
    return new OperatorComp(ANode(_lhs->cloneNode()), ANode(_rhs->cloneNode()), _sign);
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
    else if (_sign == "!=")
        result = (double)(lhs != rhs);
    else
        result = monostate();
}
//======================================================================================================================