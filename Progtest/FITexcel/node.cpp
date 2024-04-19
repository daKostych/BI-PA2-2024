#include "node.h"
#include <variant>
#include <string>
#include <memory>

using CValue = std::variant<std::monostate, double, std::string>;
using ANode = unique_ptr<Node>;
//======================================================================================================================
CValue Operand::evaluateNode() const { return _val; }
//======================================================================================================================
CValue RefOperand::evaluateNode() const {} // todo
//======================================================================================================================
CValue OperatorAdd::evaluateNode() const {} //todo
//======================================================================================================================
CValue OperatorSub::evaluateNode() const {} //todo
//======================================================================================================================
CValue OperatorMul::evaluateNode() const {} //todo
//======================================================================================================================
CValue OperatorDiv::evaluateNode() const {} //todo
//======================================================================================================================
CValue OperatorPow::evaluateNode() const {} //todo
//======================================================================================================================
CValue OperatorNeg::evaluateNode() const {} //todo
//======================================================================================================================
CValue OperatorComp::evaluateNode() const {} //todo
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
bool operator<(ANode lhs, ANode rhs){} //todo
//======================================================================================================================
bool operator==(ANode lhs, ANode rhs){} //todo
//======================================================================================================================