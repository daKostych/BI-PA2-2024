#pragma once

#include "position.h"
#include <variant>
#include <memory>
#include <map>

using CValue = std::variant<std::monostate, double, std::string>;
class Node;
using ANode = unique_ptr<Node>;

//======================================================================================================================
class Node
{
public:
    virtual ~Node() = default;
    virtual CValue evaluateNode(map<CPos, ANode> & table) const = 0;
    virtual Node * cloneNode(int columnShift, int rowShift) const = 0;
};
//======================================================================================================================
class Operand : public Node
{
public:
    Operand(CValue newValue) : _val(std::move(newValue)) {};
    CValue evaluateNode(map<CPos, ANode> & table) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;

private:
    CValue _val;
};
//======================================================================================================================
class RefOperand : public Node
{
public:
    RefOperand(CPos reference, bool rowAb, bool columnAb) : _referencedPosition(reference),
                                                            _rowAbsolute(rowAb),
                                                            _columnAbsolute(columnAb) {}
    CValue evaluateNode(map<CPos, ANode> & table) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;

private:
    CPos _referencedPosition;
    bool _rowAbsolute = false;
    bool _columnAbsolute = false;
};
//======================================================================================================================
class OperatorAdd : public Node
{
public:
    OperatorAdd(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode(map<CPos, ANode> & table) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;

private:
    ANode _lhs, _rhs;
};
//======================================================================================================================
class OperatorSub : public Node
{
public:
    OperatorSub(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode(map<CPos, ANode> & table) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;

private:
    ANode _lhs, _rhs;
};
//======================================================================================================================
class OperatorMul : public Node
{
public:
    OperatorMul(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode(map<CPos, ANode> & table) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;

private:
    ANode _lhs, _rhs;
};
//======================================================================================================================
class OperatorDiv : public Node
{
public:
    OperatorDiv(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode(map<CPos, ANode> & table) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;

private:
    ANode _lhs, _rhs;
};
//======================================================================================================================
class OperatorPow : public Node
{
public:
    OperatorPow(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode(map<CPos, ANode> & table) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;

private:
    ANode _lhs, _rhs;
};
//======================================================================================================================
class OperatorNeg : public Node
{
public:
    OperatorNeg(ANode node) : _node(std::move(node)) {}
    CValue evaluateNode(map<CPos, ANode> & table) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;

private:
    ANode _node;
};
//======================================================================================================================
class OperatorComp : public Node
{
public:
    OperatorComp(ANode l, ANode r, const std::string sign) : _lhs(std::move(l)),
                                                             _rhs(std::move(r)),
                                                             _sign(sign) {}
    CValue evaluateNode(map<CPos, ANode> & table) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;

private:
    void compare(const CValue & lhs, const CValue & rhs, CValue & result) const;

    ANode _lhs, _rhs;
    std::string _sign;
};
//======================================================================================================================