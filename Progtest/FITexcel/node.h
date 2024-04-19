#include "position.h"

using CValue = std::variant<std::monostate, double, std::string>;

//======================================================================================================================
class Node
{
public:
    virtual ~Node() = default;
    virtual CValue evaluateNode() const = 0;
};

using ANode = std::unique_ptr<Node>;
//======================================================================================================================
class Operand : public Node
{
public:
    Operand(CValue newValue) : _val(std::move(newValue)) {}
    CValue evaluateNode() const override { return _val; }

private:
    CValue _val;
};
//======================================================================================================================
class RefOperand : public Node
{
public:
    RefOperand(CValue reference) : _referencedPosition(std::move(reference)) {}
    CValue evaluateNode() const override; // todo

private:
    CValue _referencedPosition;
};
//======================================================================================================================
class OperatorAdd : public Node
{
public:
    OperatorAdd(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode() const override; //todo

private:
    ANode _lhs, _rhs;
};
//======================================================================================================================
class OperatorSub : public Node
{
public:
    OperatorSub(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode() const override; //todo

private:
    ANode _lhs, _rhs;
};
//======================================================================================================================
class OperatorMul : public Node
{
public:
    OperatorMul(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode() const override; //todo

private:
    ANode _lhs, _rhs;
};
//======================================================================================================================
class OperatorDiv : public Node
{
public:
    OperatorDiv(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode() const override; //todo

private:
    ANode _lhs, _rhs;
};
//======================================================================================================================
class OperatorPow : public Node
{
public:
    OperatorPow(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode() const override; //todo

private:
    ANode _lhs, _rhs;
};
//======================================================================================================================
class OperatorNeg : public Node
{
public:
    OperatorNeg(ANode node) : _node(std::move(node)) {}
    CValue evaluateNode() const override; //todo

private:
    ANode _node;
};
//======================================================================================================================
class OperatorComp : public Node
{
public:
    OperatorComp(ANode l, ANode r, std::string sign) : _lhs(std::move(l)),
                                                     _rhs(std::move(r)),
                                                     _sign(std::move(sign)){}
    CValue evaluateNode() const override; //todo

    friend bool operator<(ANode lhs, ANode rhs); //todo
    friend bool operator==(ANode lhs, ANode rhs); //todo

private:
    ANode _lhs, _rhs;
    std::string _sign;
};