#pragma once

#include "node.h"

//======================================================================================================================
class FunctionSum : public Node
{
public:
    FunctionSum(ANode range) : _range(std::move(range)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _range;
};
//======================================================================================================================
class FunctionCount : public Node
{
public:
    FunctionCount(ANode range) : _range(std::move(range)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _range;
};
//======================================================================================================================
class FunctionMin : public Node
{
public:
    FunctionMin(ANode range) : _range(std::move(range)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _range;
};
//======================================================================================================================
class FunctionMax : public Node
{
public:
    FunctionMax(ANode range) : _range(std::move(range)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _range;
};
//======================================================================================================================
class FunctionCountVal : public Node
{
public:
    FunctionCountVal(ANode value, ANode range) : _value(std::move(value)),
                                                 _range(std::move(range)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _value;
    ANode _range;
};
//======================================================================================================================
class FunctionIf : public Node
{
public:
    FunctionIf(ANode cond, ANode ifTrue, ANode ifFalse) : _condition(std::move(cond)),
                                                          _ifTrue(std::move(ifTrue)),
                                                          _ifFalse(std::move(ifFalse)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _condition;
    ANode _ifTrue;
    ANode _ifFalse;
};
//======================================================================================================================
