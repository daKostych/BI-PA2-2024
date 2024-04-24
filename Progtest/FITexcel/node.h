#pragma once

#include "position.h"
#include <variant>
#include <memory>
#include <map>
#include <deque>

using CValue = std::variant<std::monostate, double, std::string>;
class Node;
using ANode = unique_ptr<Node>;
using ShiftedPos = pair<unsigned, unsigned>;

//https://www.cppstories.com/2018/06/variant/
template<class... Ts>
struct overload : Ts ...{ using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

//======================================================================================================================
class Node
{
public:
    virtual ~Node() = default;
    virtual CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const = 0;
    virtual Node * cloneNode(int columnShift, int rowShift) const = 0;
    virtual void printTree(ostream & os) const = 0;

protected:
    static ShiftedPos shiftPosition(const CPos & pos, int columnShift, int rowShift, bool rowAbs, bool columnAbs);
};
//======================================================================================================================
class Operand : public Node
{
public:
    Operand(CValue newValue) : _val(std::move(newValue)) {};
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

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
    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

private:
    CPos _referencedPosition;
    bool _rowAbsolute = false;
    bool _columnAbsolute = false;
};
//======================================================================================================================
class RangeOperand : public Node
{
public:
    RangeOperand(CPos ul, CPos lr, bool ulColAb, bool ulRowAb, bool lrColAb, bool lrRowAb)
                : _upperLeft(ul), _lowerRight(lr),
                  _ulColumnAbs(ulColAb), _ulRowAbs(ulRowAb),
                  _lrColumnAbs(lrColAb), _lrRowAbs(lrRowAb) {}

    CValue evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const override;
    Node * cloneNode(int columnShift, int rowShift) const override;
    void printTree(ostream & os) const override;

    CPos _upperLeft, _lowerRight;

private:
    bool _ulColumnAbs = false,
         _ulRowAbs = false,
         _lrColumnAbs = false,
         _lrRowAbs = false;

};
//======================================================================================================================