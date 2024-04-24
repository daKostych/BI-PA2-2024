#pragma once

#include "node.h"

//======================================================================================================================
class OperatorAdd : public Node
{
public:
    OperatorAdd(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _lhs, _rhs;
};
//======================================================================================================================
class OperatorSub : public Node
{
public:
    OperatorSub(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _lhs, _rhs;
};
//======================================================================================================================
class OperatorMul : public Node
{
public:
    OperatorMul(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _lhs, _rhs;
};
//======================================================================================================================
class OperatorDiv : public Node
{
public:
    OperatorDiv(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _lhs, _rhs;
};
//======================================================================================================================
class OperatorPow : public Node
{
public:
    OperatorPow(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _lhs, _rhs;
};
//======================================================================================================================
class OperatorNeg : public Node
{
public:
    OperatorNeg(ANode node) : _node(std::move(node)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

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
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    void compare(const CValue & lhs, const CValue & rhs, CValue & result) const;

    ANode _lhs, _rhs;
    std::string _sign;
};
//======================================================================================================================