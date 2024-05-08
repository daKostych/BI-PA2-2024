#pragma once

#include "node.h"

//======================================================================================================================
// FunctionSum class represents a node that computes the sum of the number values within a specified range.
class FunctionSum : public Node
{
public:
    FunctionSum(ANode range) : _range(std::move(range)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _range; // The range over which the sum is computed
};
//======================================================================================================================
// FunctionCount class represents a node that counts the number of the defined values within a specified range.
class FunctionCount : public Node
{
public:
    FunctionCount(ANode range) : _range(std::move(range)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _range; // The range over which the count is computed
};
//======================================================================================================================
// FunctionMin class represents a node that computes the minimum number value within a specified range.
class FunctionMin : public Node
{
public:
    FunctionMin(ANode range) : _range(std::move(range)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _range; // The range over which the minimum value is computed
};
//======================================================================================================================
// FunctionMax class represents a node that computes the maximum number value within a specified range.
class FunctionMax : public Node
{
public:
    FunctionMax(ANode range) : _range(std::move(range)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _range; // The range over which the maximum value is computed
};
//======================================================================================================================
// FunctionCountVal class represents a node that counts occurrences of a specific value within a specified range.
class FunctionCountVal : public Node
{
public:
    FunctionCountVal(ANode value, ANode range) : _value(std::move(value)),
                                                 _range(std::move(range)) {}
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    ANode _value; // The value to be counted within the range
    ANode _range; // The range over which the value is counted
};
//======================================================================================================================
// FunctionIf class represents a node that performs conditional evaluation based on a condition.
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
    ANode _condition; // The condition to evaluate
    ANode _ifTrue;    // Node to evaluate if the condition is true
    ANode _ifFalse;   // Node to evaluate if the condition is false
};
//======================================================================================================================