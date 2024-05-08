#pragma once

#include "node.h"

//======================================================================================================================
// OperatorAdd class represents the addition (+) operator node in the expression tree.
class OperatorAdd : public Node
{
public:
    OperatorAdd(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _lhs; // Left operand of the addition operation.
    ANode _rhs; // Right operand of the addition operation.
};
//======================================================================================================================
// OperatorSub class represents the subtraction (-) operator node in the expression tree.
class OperatorSub : public Node
{
public:
    OperatorSub(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _lhs; // Left operand of the subtraction operation.
    ANode _rhs; // Right operand of the subtraction operation.
};
//======================================================================================================================
// OperatorMul class represents the multiplication (*) operator node in the expression tree.
class OperatorMul : public Node
{
public:
    OperatorMul(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _lhs; // Left operand of the multiplication operation.
    ANode _rhs; // Right operand of the multiplication operation.
};
//======================================================================================================================
// OperatorDiv class represents the division (/) operator node in the expression tree.
class OperatorDiv : public Node
{
public:
    OperatorDiv(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _lhs; // Left operand of the division operation.
    ANode _rhs; // Right operand of the division operation.
};
//======================================================================================================================
// OperatorPow class represents the power (^) operator node in the expression tree.
class OperatorPow : public Node
{
public:
    OperatorPow(ANode l, ANode r) : _lhs(std::move(l)), _rhs(std::move(r)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _lhs; // Left operand of the power operation.
    ANode _rhs; // Right operand of the power operation.
};
//======================================================================================================================
// OperatorNeg class represents the unary negation (-) operator node in the expression tree.
class OperatorNeg : public Node
{
public:
    OperatorNeg(ANode node) : _node(std::move(node)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _node; // Operand of the unary negation operation.
};
//======================================================================================================================
// OperatorComp class represents a comparison (>, >=, <, <=, ==, <>) operator node in the expression tree.
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
    // Helper function to perform the actual comparison based on the comparison sign.
    void compare(const CValue & lhs, const CValue & rhs, CValue & result) const;

    ANode _lhs,   // Left operand of the comparison operation.
          _rhs;   // Right operand of the comparison operation.
    string _sign; // Comparison sign (>, >=, <, <=, ==, <>).
};
//======================================================================================================================