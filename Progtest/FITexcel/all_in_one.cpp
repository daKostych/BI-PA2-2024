#ifndef __PROGTEST__
#include "expression.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <variant>
#include <optional>
#include <compare>
#include <charconv>
#include <span>
#include <utility>

using namespace std::literals;
using CValue = std::variant<std::monostate, double, std::string>;

#endif /* __PROGTEST__ */
using namespace std;
class Node;
using ANode = unique_ptr<Node>;



//======================================================================================================================
class CPos
{
public:
    // Constructors
    CPos() = default;
    explicit CPos(string_view str);
    explicit CPos(unsigned column, unsigned row) : _row(row), _column(column) {}

    // Member function to convert column to string representation
    string columnToString() const;

    // Friend function
    friend bool isValidRange(const CPos & upperLeft, const CPos & lowerRight);

    // Friend operators
    friend bool operator<(const CPos & lhs, const CPos & rhs);
    friend bool operator==(const CPos & lhs, const CPos & rhs);

    unsigned _row = 0, _column = 0;  // Member variables representing row and column
};
//======================================================================================================================
bool isValidRange(const CPos & upperLeft, const CPos & lowerRight);

//======================================================================================================================
CPos::CPos(string_view str)
{
    string index(str);

    // Find the first non-alphanumeric character
    auto position = find_if(index.begin(), index.end(), [](char & symbol)
    {
        return !isalnum(symbol);
    });

    // If non-alphanumeric character is found, throw an exception
    if (position != index.end())
        throw invalid_argument("Invalid position.");

    // Find the index where the column string ends (first digit encountered)
    size_t columnEnd = index.find_first_of("1234567890");
    unsigned calculateColumn = 0;

    // Calculate the column number
    for (size_t i = 0; i < columnEnd; i++)
    {
        char symbol = (char)toupper(index[i]);
        if (symbol < 'A' || symbol > 'Z')
            throw invalid_argument("Invalid position.");
        calculateColumn *= 26;
        calculateColumn += symbol - 'A' + 1;
    }

    // Set the column and row based on the parsed values from the input string
    _column = calculateColumn;
    _row = stoi(index.substr(columnEnd)); // Convert the remaining string (after column) to integer for row
}
//======================================================================================================================
string CPos::columnToString() const
{
    string result;
    int column = static_cast<int>(_column);

    // Convert column number to its string representation
    for (; column > 0; column = (column - 1) / 26)
        result.insert(result.begin(), (char)((column - 1) % 26 + 'A'));
    return result;
}
//======================================================================================================================
// Check if the range is valid
bool isValidRange(const CPos & upperLeft, const CPos & lowerRight)
{
    return (upperLeft._column <= lowerRight._column) &&
           (upperLeft._row <= lowerRight._row);
}
//======================================================================================================================
bool operator<(const CPos & lhs, const CPos & rhs)
{
    // Compare positions based on column first, then row if columns are equal
    if (lhs._column != rhs._column)
        return lhs._column < rhs._column;
    return lhs._row < rhs._row;
}
//======================================================================================================================
bool operator==(const CPos & lhs, const CPos & rhs)
{
    // Check if two positions are identical (same row and column)
    return lhs._row == rhs._row &&
           lhs._column == rhs._column;
}
//======================================================================================================================


using ShiftedPos = pair<unsigned, unsigned>;

// https://www.cppstories.com/2018/06/variant/
// Template for creating overloaded lambdas
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
    CValue _val;  // Value held by Operand
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
    CPos _referencedPosition;      // Referenced position
    bool _rowAbsolute = false;     // Flag indicating row absolute
    bool _columnAbsolute = false;  // Flag indicating column absolute
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

    CPos _upperLeft, _lowerRight;  // Upper-left and lower-right positions

private:
    bool _ulColumnAbs = false,     // Flag indicating upper-left column absolute
         _ulRowAbs = false,        // Flag indicating upper-left row absolute
         _lrColumnAbs = false,     // Flag indicating lower-right column absolute
         _lrRowAbs = false;        // Flag indicating lower-right row absolute
};
//======================================================================================================================

//======================================================================================================================
// Print the value of an Operand node to the output stream.
void Operand::printTree(ostream & os) const
{
    visit(overload
    {
        [&os](double val) { os << val; },
        [&os](const string & val)
        {
            os << "\"";
            for (char c : val)
            {
                if (c == '"')
                    os << "\"\"";
                else
                    os << c;
            }
            os << "\"";
        },
        [](const auto & val) {}
    }, _val);
}
//======================================================================================================================
// Print the referenced position of a RefOperand node to the output stream.
void RefOperand::printTree(ostream & os) const
{
    if (_columnAbsolute)
        os << '$';
    os << _referencedPosition.columnToString();
    if (_rowAbsolute)
        os << '$';
    os << _referencedPosition._row;
}

//======================================================================================================================
// Print the range boundaries of a RangeOperand node to the output stream.
void RangeOperand::printTree(ostream & os) const
{
    if (_ulColumnAbs)
        os << '$';
    os << _upperLeft.columnToString();
    if (_ulRowAbs)
        os << '$';
    os << _upperLeft._row;
    os << ":";
    if (_lrColumnAbs)
        os << '$';
    os << _lowerRight.columnToString();
    if (_lrRowAbs)
        os << '$';
    os << _lowerRight._row;
}
//======================================================================================================================
// Evaluate the value of an Operand node.
CValue Operand::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const { return _val; }
//======================================================================================================================
// Evaluate the value referenced by a RefOperand node from a given table of values.
CValue RefOperand::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    auto iterator = table.find(_referencedPosition);
    if (iterator == table.end())
        return monostate();

    auto callerPos = find(callers.begin(), callers.end(), _referencedPosition);
    if (callerPos != callers.end())
        return monostate();
    callers.push_back(_referencedPosition);
    CValue result = iterator->second->evaluateNode(table, callers);
    callers.pop_back();
    return result;
}
//======================================================================================================================
// Evaluate the value of a RangeOperand node.
CValue RangeOperand::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const { return monostate(); }
//======================================================================================================================
// Clone an Operand node.
Node * Operand::cloneNode(int columnShift, int rowShift) const { return new Operand(_val); }
//======================================================================================================================
// Clone a RefOperand node with optional column and row shifts.
Node * RefOperand::cloneNode(int columnShift, int rowShift) const
{
    ShiftedPos shifted = shiftPosition(_referencedPosition,
                                       columnShift, rowShift,
                                       _rowAbsolute, _columnAbsolute);

    return new RefOperand(CPos(shifted.first, shifted.second),
                          _rowAbsolute,
                          _columnAbsolute);
}
//======================================================================================================================
// Clone a RangeOperand node with optional column and row shifts.
Node * RangeOperand::cloneNode(int columnShift, int rowShift) const
{
    ShiftedPos shiftedUpperLeft = shiftPosition(_upperLeft,
                                                columnShift, rowShift,
                                                _ulRowAbs, _ulColumnAbs);

    ShiftedPos shiftedLowerRight = shiftPosition(_lowerRight,
                                                 columnShift, rowShift,
                                                 _lrRowAbs, _lrColumnAbs);

    return new RangeOperand(CPos(shiftedUpperLeft.first, shiftedUpperLeft.second),
                            CPos(shiftedLowerRight.first, shiftedLowerRight.second),
                            _ulColumnAbs, _ulRowAbs,
                            _lrColumnAbs, _lrRowAbs);
}
//======================================================================================================================
// Utility function to shift a position by specified column and row shifts based on absolute flags.
ShiftedPos Node::shiftPosition(const CPos & pos, int columnShift, int rowShift, bool rowAbs, bool columnAbs)
{
    unsigned newRow = pos._row,
             newColumn = pos._column;
    if (!rowAbs)
        newRow += rowShift;
    if (!columnAbs)
        newColumn += columnShift;
    return make_pair(newColumn, newRow);
}


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

//======================================================================================================================
// Print the expression tree for a sum function
void FunctionSum::printTree(ostream & os) const
{
    os << "sum(";
    _range->printTree(os);
    os << ")";
}
//======================================================================================================================
// Print the expression tree for a count function
void FunctionCount::printTree(ostream & os) const
{
    os << "count(";
    _range->printTree(os);
    os << ")";
}
//======================================================================================================================
// Print the expression tree for a min function
void FunctionMin::printTree(ostream & os) const
{
    os << "min(";
    _range->printTree(os);
    os << ")";
}
//======================================================================================================================
// Print the expression tree for a max function
void FunctionMax::printTree(ostream & os) const
{
    os << "max(";
    _range->printTree(os);
    os << ")";
}
//======================================================================================================================
// Print the expression tree for a countval function
void FunctionCountVal::printTree(ostream & os) const
{
    os << "countval(";
    _value->printTree(os);
    os << ", ";
    _range->printTree(os);
    os << ")";
}
//======================================================================================================================
// Print the expression tree for an if function
void FunctionIf::printTree(ostream & os) const
{
    os << "if(";
    _condition->printTree(os);
    os << ", ";
    _ifTrue->printTree(os);
    os << ", ";
    _ifFalse->printTree(os);
    os << ")";
}
//======================================================================================================================
// Evaluate the result of a sum function
CValue FunctionSum::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    double result = 0.0;
    bool foundNumber = false;
    auto range = _range->cloneNode(0, 0);
    auto copyRange = dynamic_cast<RangeOperand*>(range);
    for (auto column = copyRange->_upperLeft._column; column <= copyRange->_lowerRight._column; column++)
    {
        for (auto row = copyRange->_upperLeft._row; row <= copyRange->_lowerRight._row; row++)
        {
            auto position = table.find(CPos(column, row));
            if (position == table.end())
                continue;
            CValue tmp = position->second->evaluateNode(table, callers);
            visit(overload
            {
                [&result, &foundNumber](double value)
                {
                    result += value;
                    foundNumber = true;
                },
                [](const auto & value) { /* Handle non-double values */ }
            }, tmp);
        }
    }
    delete range;
    if (foundNumber)
        return {result};
    else
        return {};
}
//======================================================================================================================
// Evaluate the result of a count function
CValue FunctionCount::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    double count = 0.0;
    auto range = _range->cloneNode(0, 0);
    auto copyRange = dynamic_cast<RangeOperand*>(range);
    for (auto column = copyRange->_upperLeft._column; column <= copyRange->_lowerRight._column; column++)
    {
        for (auto row = copyRange->_upperLeft._row; row <= copyRange->_lowerRight._row; row++)
        {
            auto position = table.find(CPos(column, row));
            if (position == table.end())
                continue;
            CValue tmp = position->second->evaluateNode(table, callers);
            visit(overload
            {
                [&count](double value) { count++; },
                [&count](const string & value) { count++; },
                [](const auto & value) { /* Handle non-double values */ }
            }, tmp);
        }
    }
    delete range;
    return {count};
}
//======================================================================================================================
// Evaluate the result of a min function
CValue FunctionMin::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    auto min = DBL_MAX;
    bool foundNumber = false;
    auto range = _range->cloneNode(0, 0);
    auto copyRange = dynamic_cast<RangeOperand*>(range);
    for (auto column = copyRange->_upperLeft._column; column <= copyRange->_lowerRight._column; column++)
    {
        for (auto row = copyRange->_upperLeft._row; row <= copyRange->_lowerRight._row; row++)
        {
            auto position = table.find(CPos(column, row));
            if (position == table.end())
                continue;
            CValue tmp = position->second->evaluateNode(table, callers);
            visit(overload
            {
                [&min, &foundNumber](double value)
                {
                    min = (value < min) ? value : min;
                    foundNumber = true;
                },
                [](const auto & value) { /* Handle non-double values */ }
            }, tmp);
        }
    }
    delete range;
    if (foundNumber)
        return {min};
    else
        return {};
}
//======================================================================================================================
// Evaluate the result of a max function
CValue FunctionMax::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    auto max = DBL_MIN;
    bool foundNumber = false;
    auto range = _range->cloneNode(0, 0);
    auto copyRange = dynamic_cast<RangeOperand*>(range);
    for (auto column = copyRange->_upperLeft._column; column <= copyRange->_lowerRight._column; column++)
    {
        for (auto row = copyRange->_upperLeft._row; row <= copyRange->_lowerRight._row; row++)
        {
            auto position = table.find(CPos(column, row));
            if (position == table.end())
                continue;
            CValue tmp = position->second->evaluateNode(table, callers);
            visit(overload
            {
                [&max, &foundNumber](double value)
                {
                    max = (value > max) ? value : max;
                    foundNumber = true;
                },
                [](const auto & value) { /* Handle non-double values */ }
            }, tmp);
        }
    }
    delete range;
    if (foundNumber)
        return {max};
    else
        return {};
}
//======================================================================================================================
// Evaluate the result of a countval function
CValue FunctionCountVal::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    double count = 0.0;
    CValue valueToCount = _value->evaluateNode(table, callers);
    auto range = _range->cloneNode(0, 0);
    auto copyRange = dynamic_cast<RangeOperand*>(range);
    for (auto column = copyRange->_upperLeft._column; column <= copyRange->_lowerRight._column; column++)
    {
        for (auto row = copyRange->_upperLeft._row; row <= copyRange->_lowerRight._row; row++)
        {
            auto position = table.find(CPos(column, row));
            if (position == table.end())
                continue;
            CValue tmp = position->second->evaluateNode(table, callers);
            visit(overload
            {
                [&count] (double val1, double val2){ count = (val1 == val2) ? count + 1 : count; },
                [&count] (const string & val1, const string & val2) { count = (val1 == val2) ? count + 1
                                                                                                    : count; },
                [] (const auto & val1 , const auto & val2) { /* Handle other value types */ }
            }, tmp, valueToCount);
        }
    }
    delete range;
    return {count};
}
//======================================================================================================================
// Evaluate the result of an if function
CValue FunctionIf::evaluateNode(map<CPos, ANode> & table, deque<CPos> & callers) const
{
    CValue condition = _condition->evaluateNode(table, callers);
    CValue result;
    visit(overload
    {
        [&](double value)
        {
            if (value != 0.0)
                result = _ifTrue->evaluateNode(table, callers);
            else
                result = _ifFalse->evaluateNode(table, callers);
        },
        [&result] (const auto & value) { result = monostate(); } // Default result for non-double values
    }, condition);
    return result;
}
//======================================================================================================================
// Clone the node for a sum function
Node * FunctionSum::cloneNode(int columnShift, int rowShift) const
{
    return new FunctionSum(ANode(_range->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
// Clone the node for a count function
Node * FunctionCount::cloneNode(int columnShift, int rowShift) const
{
    return new FunctionCount(ANode(_range->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
// Clone the node for a min function
Node * FunctionMin::cloneNode(int columnShift, int rowShift) const
{
    return new FunctionMin(ANode(_range->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
// Clone the node for a max function
Node * FunctionMax::cloneNode(int columnShift, int rowShift) const
{
    return new FunctionMax(ANode(_range->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
// Clone the node for a countval function
Node * FunctionCountVal::cloneNode(int columnShift, int rowShift) const
{
    return new FunctionCountVal(ANode(_value->cloneNode(columnShift, rowShift)),
                                ANode(_range->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================
// Clone the node for an if function
Node * FunctionIf::cloneNode(int columnShift, int rowShift) const
{
    return new FunctionIf(ANode(_condition->cloneNode(columnShift, rowShift)),
                          ANode(_ifTrue->cloneNode(columnShift, rowShift)),
                          ANode(_ifFalse->cloneNode(columnShift, rowShift)));
}
//======================================================================================================================


class Builder : public CExprBuilder
{
public:
    Builder() : CExprBuilder() {};

    // Override methods from CExprBuilder for building expressions
    void opAdd() override;
    void opSub() override;
    void opMul() override;
    void opDiv() override;
    void opPow() override;
    void opNeg() override;
    void opEq() override;
    void opNe() override;
    void opLt() override;
    void opLe() override;
    void opGt() override;
    void opGe() override;
    void valNumber(double val) override;
    void valString(std::string val) override;
    void valReference(std::string val) override;
    void valRange(std::string val) override;
    void funcCall(std::string fnName, int paramCount) override;

    stack<ANode> builderStack; // Stack to manage expression nodes

private:
    static string removeDollars(const string & str);
};

//======================================================================================================================
// Operator: Addition
void Builder::opAdd()
{
    // Pop right-hand side and left-hand side operands from the stack
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();

    // Create an OperatorAdd node and emplace it onto the stack
    builderStack.emplace(new OperatorAdd(std::move(lhs), std::move(rhs)));
}
//======================================================================================================================
// Operator: Subtraction
void Builder::opSub()
{
    // Pop right-hand side and left-hand side operands from the stack
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();

    // Create an OperatorSub node and emplace it onto the stack
    builderStack.emplace(new OperatorSub(std::move(lhs), std::move(rhs)));
}
//======================================================================================================================
// Operator: Multiplication
void Builder::opMul()
{
    // Pop right-hand side and left-hand side operands from the stack
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();

    // Create an OperatorMul node and emplace it onto the stack
    builderStack.emplace(new OperatorMul(std::move(lhs), std::move(rhs)));
}
//======================================================================================================================
// Operator: Division
void Builder::opDiv()
{
    // Pop right-hand side and left-hand side operands from the stack
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();

    // Create an OperatorDiv node and emplace it onto the stack
    builderStack.emplace(new OperatorDiv(std::move(lhs), std::move(rhs)));
}
//======================================================================================================================
// Operator: Power
void Builder::opPow()
{
    // Pop right-hand side and left-hand side operands from the stack
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();

    // Create an OperatorPow node and emplace it onto the stack
    builderStack.emplace(new OperatorPow(std::move(lhs), std::move(rhs)));
}
//======================================================================================================================
// Operator: Negation
void Builder::opNeg()
{
    // Pop the operand from the stack
    ANode node = std::move(builderStack.top());
    builderStack.pop();

    // Create an OperatorNeg node and emplace it onto the stack
    builderStack.emplace(new OperatorNeg(std::move(node)));
}
//======================================================================================================================
// Operator: Equal
void Builder::opEq()
{
    // Pop right-hand side and left-hand side operands from the stack
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();

    // Create an OperatorComp (==) node and emplace it onto the stack
    builderStack.emplace(new OperatorComp(std::move(lhs), std::move(rhs), "=="));
}
//======================================================================================================================
// Operator: Not Equal
void Builder::opNe()
{
    // Pop right-hand side and left-hand side operands from the stack
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();

    // Create an OperatorComp (<>) node and emplace it onto the stack
    builderStack.emplace(new OperatorComp(std::move(lhs), std::move(rhs), "<>"));
}
//======================================================================================================================
// Operator: Less Than
void Builder::opLt()
{
    // Pop right-hand side and left-hand side operands from the stack
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();

    // Create an OperatorComp (<) node and emplace it onto the stack
    builderStack.emplace(new OperatorComp(std::move(lhs), std::move(rhs), "<"));
}
//======================================================================================================================
// Operator: Less Than or Equal
void Builder::opLe()
{
    // Pop right-hand side and left-hand side operands from the stack
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();

    // Create an OperatorComp (<=) node and emplace it onto the stack
    builderStack.emplace(new OperatorComp(std::move(lhs), std::move(rhs), "<="));
}
//======================================================================================================================
// Operator: Greater Than
void Builder::opGt()
{
    // Pop right-hand side and left-hand side operands from the stack
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();

    // Create an OperatorComp (>) node and emplace it onto the stack
    builderStack.emplace(new OperatorComp(std::move(lhs), std::move(rhs), ">"));
}
//======================================================================================================================
// Operator: Greater Than or Equal
void Builder::opGe()
{
    // Pop right-hand side and left-hand side operands from the stack
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();

    // Create an OperatorComp (>=) node and emplace it onto the stack
    builderStack.emplace(new OperatorComp(std::move(lhs), std::move(rhs), ">="));
}
//======================================================================================================================
// Operand: Number
void Builder::valNumber(double val)
{
    // Create an Operand node with the specified value and push it onto the stack
    builderStack.emplace(new Operand(val));
}

//======================================================================================================================
// Operand: String
void Builder::valString(std::string val)
{
    // Create an Operand node with the specified string value and emplace it onto the stack
    builderStack.emplace(new Operand(val));
}

//======================================================================================================================
// Operand: Reference
void Builder::valReference(std::string val)
{
    // Determine if the reference is absolute or relative and create a RefOperand node accordingly
    bool rowAb = val.substr(1).find('$') != string::npos;
    bool columnAb = val[0] == '$';
    if (rowAb || columnAb)
    {
        string relativePos = removeDollars(val);
        builderStack.emplace(new RefOperand(CPos(relativePos), rowAb, columnAb));
    }
    else
        builderStack.emplace(new RefOperand(CPos(val), rowAb, columnAb));
}
//======================================================================================================================
// Operand: Range
void Builder::valRange(std::string val)
{
    // Parse the range string and create a RangeOperand node
    size_t colonPos = val.find(':');
    bool upperLeftRowAb = val.substr(1, colonPos - 1).find('$') != string::npos;
    bool upperLeftColumnAb = val[0] == '$';
    bool lowerRightRowAb = val.substr(colonPos + 2).find('$') != string::npos;
    bool lowerRightColumnAb = val[colonPos + 1] == '$';
    CPos upperLeft(removeDollars(val.substr(0, colonPos)));
    CPos lowerRight(removeDollars(val.substr(colonPos + 1)));

    // Validate the range and create a RangeOperand node
    if (!isValidRange(upperLeft, lowerRight))
        throw logic_error("Invalid range!");
    builderStack.emplace(new RangeOperand(upperLeft, lowerRight,
                                             upperLeftColumnAb, upperLeftRowAb,
                                             lowerRightColumnAb, lowerRightRowAb));
}
//======================================================================================================================
// Function Call
void Builder::funcCall(std::string fnName, int paramCount)
{
    // Handle different built-in function calls
    if (fnName == "sum")
    {
        ANode range = std::move(builderStack.top());
        builderStack.pop();
        builderStack.emplace(new FunctionSum(std::move(range)));
    }
    else if (fnName == "min")
    {
        ANode range = std::move(builderStack.top());
        builderStack.pop();
        builderStack.emplace(new FunctionMin(std::move(range)));
    }
    else if (fnName == "max")
    {
        ANode range = std::move(builderStack.top());
        builderStack.pop();
        builderStack.emplace(new FunctionMax(std::move(range)));
    }
    else if (fnName == "count")
    {
        ANode range = std::move(builderStack.top());
        builderStack.pop();
        builderStack.emplace(new FunctionCount(std::move(range)));
    }
    else if (fnName == "countval")
    {
        ANode range = std::move(builderStack.top());
        builderStack.pop();
        ANode value = std::move(builderStack.top());
        builderStack.pop();
        builderStack.emplace(new FunctionCountVal(std::move(value), std::move(range)));
    }
    else
    {
        // Handle function IF(condition, ifTrue, ifFalse)
        ANode ifFalse = std::move(builderStack.top());
        builderStack.pop();
        ANode ifTrue = std::move(builderStack.top());
        builderStack.pop();
        ANode condition = std::move(builderStack.top());
        builderStack.pop();
        builderStack.emplace(new FunctionIf(std::move(condition),
                                                    std::move(ifTrue),
                                                    std::move(ifFalse)));
    }
}
//======================================================================================================================
// Removing the '$' sign from the cell ID
string Builder::removeDollars(const string & str)
{
    string result;
    int dollarCnt = 0;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] != '$')
            result += str[i];
        else
            i != 0 ? (str[i - 1] == '$' ? dollarCnt += 0
                                        : dollarCnt++)
                   : dollarCnt++;
    }
    if (dollarCnt > 2)
        throw invalid_argument("Invalid reference!");
    return result;
}
//======================================================================================================================



//======================================================================================================================
class CSpreadsheet
{
public:
    // Static function to return supported capabilities
    static unsigned capabilities()
    {
        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS;
    }

    // Constructors and assignment operators
    CSpreadsheet() = default;                                 // Default constructor
    CSpreadsheet(const CSpreadsheet & other);                 // Copy constructor
    CSpreadsheet(CSpreadsheet && other) noexcept;             // Move constructor
    CSpreadsheet & operator=(const CSpreadsheet & other);     // Copy assignment operator
    CSpreadsheet & operator=(CSpreadsheet && other) noexcept; // Move assignment operator

    // Load spreadsheet from input stream
    bool load(std::istream & is);

    // Static method to load spreadsheet from a specified cell
    static bool loadSell(const string & cell, CSpreadsheet & spreadSheet);

    // Save spreadsheet to output stream
    bool save(std::ostream & os) const;

    // Set the contents of a cell at the specified position
    bool setCell(CPos pos, std::string contents);

    // Get the value stored in the cell at the specified position
    CValue getValue(CPos pos);

    // Copy a rectangular area of cells from source to destination
    void copyRect(CPos dst, CPos src, int w = 1, int h = 1);

private:
    std::map<CPos, ANode> _table; // Internal representation of the spreadsheet
};
//======================================================================================================================

//======================================================================================================================
CSpreadsheet::CSpreadsheet(const CSpreadsheet & other)
{
    _table.clear(); // Clear the current table
    // Iterate over each element in the other table and clone each node, then add to _table
    for (const auto & element : other._table)
        _table.emplace(element.first, element.second->cloneNode(0, 0));
}
//======================================================================================================================
CSpreadsheet::CSpreadsheet(CSpreadsheet && other) noexcept
{
    _table = std::move(other._table); // Move other's table to this object
}
//======================================================================================================================
CSpreadsheet & CSpreadsheet::operator=(const CSpreadsheet & other)
{
    if (this == &other)
        return *this;

    // Create a temporary copy of other using copy constructor
    CSpreadsheet copyToSwap(other);
    // Swap the content of _table with the temporary table
    swap(_table, copyToSwap._table);
    return *this;
}
//======================================================================================================================
CSpreadsheet & CSpreadsheet::operator=(CSpreadsheet && other) noexcept
{
    if (this == &other)
        return *this;

    _table = std::move(other._table); // Move other's table to this object
    return *this;
}
//======================================================================================================================
bool CSpreadsheet::loadSell(const string & cell, CSpreadsheet & spreadSheet)
{
    string column, row, value;
    if (cell[0] != 'c')
        return false;

    size_t index = 1;

    // Extract column from the cell string
    for (; index < cell.size() && cell[index] != 'r'; index++)
    {
        if (cell[index] < 'A' || cell[index] > 'Z')
            return false;
        column += cell[index];
    }
    index++;

    // Extract row from the cell string
    for (; index < cell.size() && cell[index] != '_'; index++)
    {
        if (cell[index] < '0' || cell[index] > '9')
            return false;
        row += cell[index];
    }
    index++;

    // Extract value from the cell string
    value = cell.substr(index);

    try
    {
        // Set the cell contents in the provided spreadsheet object
        spreadSheet.setCell(CPos(column + row), value);
        return true;
    }
    catch (const exception & ex ) { return false; }
}
//======================================================================================================================
bool CSpreadsheet::load(std::istream & is)
{
    CSpreadsheet newSpreadSheet; // Create a new temporary spreadsheet
    string fullTable, tablePart;

    // Read the entire table from the input stream
    while (getline(is, tablePart))
        fullTable += tablePart + '\n';
    fullTable.erase(fullTable.size()-1);

    // Parse the full table and load into the temporary spreadsheet
    for(size_t i = 0, from = 0; i < fullTable.size(); i++)
    {
        if (fullTable[i] == '|')
        {
            // Load each cell defined by the substring between '|' characters
            if (!loadSell(fullTable.substr(from, i - from), newSpreadSheet))
                return false;
            from = i + 1;
        }
    }
    // Move the table from the temporary spreadsheet to this object
    _table = std::move(newSpreadSheet._table);
    return true;
}
//======================================================================================================================
bool CSpreadsheet::save(std::ostream & os) const
{
    // Iterate over each element in _table and save to output stream
    for (const auto & element : _table)
    {
        os << "c" + element.first.columnToString() +
              "r" + to_string(element.first._row) +
              "_=";
        element.second->printTree(os);
        os << '|';
    }
    return true;
}
//======================================================================================================================
bool CSpreadsheet::setCell(CPos pos, std::string contents)
{
    try
    {
        Builder builder;
        parseExpression(std::move(contents), builder);  // Parse the contents using function parseExpression
        _table[pos] = std::move(builder.builderStack.top());   // Set the cell value
    }
    catch (const exception & ex)
    {
        return false;
    }
    return true;
}
//======================================================================================================================
CValue CSpreadsheet::getValue(CPos pos)
{
    auto iterator = _table.find(pos);
    if (iterator == _table.end())
        return monostate(); // Return monostate if cell is not found
    deque<CPos> callers;
    callers.push_back(pos);
    // Evaluate the node stored in the cell and return the value
    return _table[pos]->evaluateNode(_table, callers);
}
//======================================================================================================================
void CSpreadsheet::copyRect(CPos dst, CPos src, int w, int h)
{
    if (w < 1 || h < 1 || dst == src)
        return;

    map<CPos, ANode> tmpTable;
    int columnShift = static_cast<int>(max(dst._column, src._column) - min(dst._column, src._column));
    if (max(dst._column, src._column) == src._column)
        columnShift = -columnShift;
    int rowShift = static_cast<int>(max(dst._row, src._row) - min(dst._row, src._row));
    if (max(dst._row, src._row) == src._row)
        rowShift = -rowShift;

    // Iterate over the specified rectangular area and copy cells with adjusted positions
    for (size_t column = src._column; column < src._column + w; column++)
    {
        for (size_t row = src._row; row < src._row + h; row++)
        {
            auto iterator = _table.find(CPos(column, row));
            if (iterator != _table.end())
                tmpTable.emplace(CPos(column + columnShift, row + rowShift),
                                 _table[CPos(column, row)]->cloneNode(columnShift, rowShift));
        }
    }

    // Move the copied cells back to the original table
    for (auto & element : tmpTable)
        _table[element.first] = std::move(element.second);
}
//======================================================================================================================
#ifndef __PROGTEST__

bool valueMatch(const CValue & r, const CValue & s)
{
    if (r.index() != s.index())
        return false;
    if (r.index() == 0)
        return true;
    if (r.index() == 2)
        return std::get<std::string>(r) == std::get<std::string>(s);
    if (std::isnan(std::get<double>(r)) && std::isnan(std::get<double>(s)))
        return true;
    if (std::isinf(std::get<double>(r)) && std::isinf(std::get<double>(s)))
        return (std::get<double>(r) < 0 && std::get<double>(s) < 0) ||
               (std::get<double>(r) > 0 && std::get<double>(s) > 0);
    return fabs(std::get<double>(r) - std::get<double>(s)) <= 1e8 * DBL_EPSILON * fabs(std::get<double>(r));
}

int main()
{
    CSpreadsheet x0, x1;
    std::ostringstream oss;
    std::istringstream iss;
    std::string data;
    assert(x0.setCell(CPos("A1"), "10"));
    assert(x0.setCell(CPos("A2"), "20.5"));
    assert(x0.setCell(CPos("A3"), "3e1"));
    assert(x0.setCell(CPos("A4"), "=40"));
    assert(x0.setCell(CPos("A5"), "=5e+1"));
    assert(x0.setCell(CPos("A6"), "raw text with any characters, including a quote \" or a newline\n"));
    assert(x0.setCell(CPos("A7"),
                      "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\""));
    assert(valueMatch(x0.getValue(CPos("A1")), CValue(10.0)));
    assert(valueMatch(x0.getValue(CPos("A2")), CValue(20.5)));
    assert(valueMatch(x0.getValue(CPos("A3")), CValue(30.0)));
    assert(valueMatch(x0.getValue(CPos("A4")), CValue(40.0)));
    assert(valueMatch(x0.getValue(CPos("A5")), CValue(50.0)));
    assert(valueMatch(x0.getValue(CPos("A6")),
                      CValue("raw text with any characters, including a quote \" or a newline\n")));
    assert(valueMatch(x0.getValue(CPos("A7")),
                      CValue("quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++.")));
    assert(valueMatch(x0.getValue(CPos("A8")), CValue()));
    assert(valueMatch(x0.getValue(CPos("AAAA9999")), CValue()));
    assert(x0.setCell(CPos("B1"), "=A1+A2*A3"));
    assert(x0.setCell(CPos("B2"), "= -A1 ^ 2 - A2 / 2   "));
    assert(x0.setCell(CPos("B3"), "= 2 ^ $A$1"));
    assert(x0.setCell(CPos("B4"), "=($A1+A$2)^2"));
    assert(x0.setCell(CPos("B5"), "=B1+B2+B3+B4"));
    assert(x0.setCell(CPos("B6"), "=B1+B2+B3+B4+B5"));
    assert(valueMatch(x0.getValue(CPos("B1")), CValue(625.0)));
    assert(valueMatch(x0.getValue(CPos("B2")), CValue(-110.25)));
    assert(valueMatch(x0.getValue(CPos("B3")), CValue(1024.0)));
    assert(valueMatch(x0.getValue(CPos("B4")), CValue(930.25)));
    assert(valueMatch(x0.getValue(CPos("B5")), CValue(2469.0)));
    assert(valueMatch(x0.getValue(CPos("B6")), CValue(4938.0)));
    assert(x0.setCell(CPos("A1"), "12"));
    assert(valueMatch(x0.getValue(CPos("B1")), CValue(627.0)));
    assert(valueMatch(x0.getValue(CPos("B2")), CValue(-154.25)));
    assert(valueMatch(x0.getValue(CPos("B3")), CValue(4096.0)));
    assert(valueMatch(x0.getValue(CPos("B4")), CValue(1056.25)));
    assert(valueMatch(x0.getValue(CPos("B5")), CValue(5625.0)));
    assert(valueMatch(x0.getValue(CPos("B6")), CValue(11250.0)));
    x1 = x0;
    assert(x0.setCell(CPos("A2"), "100"));
    assert(x1.setCell(CPos("A2"), "=A3+A5+A4"));
    assert(valueMatch(x0.getValue(CPos("B1")), CValue(3012.0)));
    assert(valueMatch(x0.getValue(CPos("B2")), CValue(-194.0)));
    assert(valueMatch(x0.getValue(CPos("B3")), CValue(4096.0)));
    assert(valueMatch(x0.getValue(CPos("B4")), CValue(12544.0)));
    assert(valueMatch(x0.getValue(CPos("B5")), CValue(19458.0)));
    assert(valueMatch(x0.getValue(CPos("B6")), CValue(38916.0)));
    assert(valueMatch(x1.getValue(CPos("B1")), CValue(3612.0)));
    assert(valueMatch(x1.getValue(CPos("B2")), CValue(-204.0)));
    assert(valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
    assert(valueMatch(x1.getValue(CPos("B4")), CValue(17424.0)));
    assert(valueMatch(x1.getValue(CPos("B5")), CValue(24928.0)));
    assert(valueMatch(x1.getValue(CPos("B6")), CValue(49856.0)));
    oss.clear();
    oss.str("");
    assert(x0.save(oss));
    data = oss.str();
    iss.clear();
    iss.str(data);
    assert(x1.load(iss));

    assert(valueMatch(x1.getValue(CPos("A6")),
                      CValue("raw text with any characters, including a quote \" or a newline\n")));
    assert(valueMatch(x1.getValue(CPos("A7")),
                      CValue("quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++.")));

    assert(valueMatch(x1.getValue(CPos("B1")), CValue(3012.0)));
    assert(valueMatch(x1.getValue(CPos("B2")), CValue(-194.0)));
    assert(valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
    assert(valueMatch(x1.getValue(CPos("B4")), CValue(12544.0)));
    assert(valueMatch(x1.getValue(CPos("B5")), CValue(19458.0)));
    assert(valueMatch(x1.getValue(CPos("B6")), CValue(38916.0)));
    assert(x0.setCell(CPos("A3"), "4e1"));
    assert(valueMatch(x1.getValue(CPos("B1")), CValue(3012.0)));
    assert(valueMatch(x1.getValue(CPos("B2")), CValue(-194.0)));
    assert(valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
    assert(valueMatch(x1.getValue(CPos("B4")), CValue(12544.0)));
    assert(valueMatch(x1.getValue(CPos("B5")), CValue(19458.0)));
    assert(valueMatch(x1.getValue(CPos("B6")), CValue(38916.0)));
    oss.clear();
    oss.str("");
    assert(x0.save(oss));
    data = oss.str();
    for (size_t i = 0; i < std::min<size_t>(data.length(), 10); i++)
        data[i] ^= 0x5a;
    iss.clear();
    iss.str(data);
    assert(!x1.load(iss));
    assert(x0.setCell(CPos("D0"), "10"));
    assert(x0.setCell(CPos("D1"), "20"));
    assert(x0.setCell(CPos("D2"), "30"));
    assert(x0.setCell(CPos("D3"), "40"));
    assert(x0.setCell(CPos("D4"), "50"));
    assert(x0.setCell(CPos("E0"), "60"));
    assert(x0.setCell(CPos("E1"), "70"));
    assert(x0.setCell(CPos("E2"), "80"));
    assert(x0.setCell(CPos("E3"), "90"));
    assert(x0.setCell(CPos("E4"), "100"));
    assert(x0.setCell(CPos("F10"), "=D0+5"));
    assert(x0.setCell(CPos("F11"), "=$D0+5"));
    assert(x0.setCell(CPos("F12"), "=D$0+5"));
    assert(x0.setCell(CPos("F13"), "=$D$0+5"));
    x0.copyRect(CPos("G11"), CPos("F10"), 1, 4);
    assert(valueMatch(x0.getValue(CPos("F10")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("F11")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("F12")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("F13")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("F14")), CValue()));
    assert(valueMatch(x0.getValue(CPos("G10")), CValue()));
    assert(valueMatch(x0.getValue(CPos("G11")), CValue(75.0)));
    assert(valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));
    assert(valueMatch(x0.getValue(CPos("G13")), CValue(65.0)));
    assert(valueMatch(x0.getValue(CPos("G14")), CValue(15.0)));
    x0.copyRect(CPos("G11"), CPos("F10"), 2, 4);
    assert(valueMatch(x0.getValue(CPos("F10")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("F11")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("F12")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("F13")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("F14")), CValue()));
    assert(valueMatch(x0.getValue(CPos("G10")), CValue()));
    assert(valueMatch(x0.getValue(CPos("G11")), CValue(75.0)));
    assert(valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));
    assert(valueMatch(x0.getValue(CPos("G13")), CValue(65.0)));
    assert(valueMatch(x0.getValue(CPos("G14")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("H10")), CValue()));
    assert(valueMatch(x0.getValue(CPos("H11")), CValue()));
    assert(valueMatch(x0.getValue(CPos("H12")), CValue()));
    assert(valueMatch(x0.getValue(CPos("H13")), CValue(35.0)));
    assert(valueMatch(x0.getValue(CPos("H14")), CValue()));
    assert(x0.setCell(CPos("F0"), "-27"));
    assert(valueMatch(x0.getValue(CPos("H14")), CValue(-22.0)));
    x0.copyRect(CPos("H12"), CPos("H13"), 1, 2);
    assert(valueMatch(x0.getValue(CPos("H12")), CValue(25.0)));
    assert(valueMatch(x0.getValue(CPos("H13")), CValue(-22.0)));
    assert(valueMatch(x0.getValue(CPos("H14")), CValue(-22.0)));


    oss.clear();
    oss.str("");
    assert(x0.save(oss));
    data = oss.str();
    iss.clear();
    iss.str(data);
    assert(x1.load(iss));

    assert(valueMatch(x1.getValue(CPos("F10")), CValue(15.0)));
    assert(valueMatch(x1.getValue(CPos("F11")), CValue(15.0)));
    assert(valueMatch(x1.getValue(CPos("F12")), CValue(15.0)));
    assert(valueMatch(x1.getValue(CPos("F13")), CValue(15.0)));
    assert(valueMatch(x1.getValue(CPos("F14")), CValue()));
    assert(valueMatch(x1.getValue(CPos("G10")), CValue()));
    assert(valueMatch(x1.getValue(CPos("G11")), CValue(75.0)));
    assert(valueMatch(x1.getValue(CPos("G12")), CValue(25.0)));
    assert(valueMatch(x1.getValue(CPos("G13")), CValue(65.0)));
    assert(valueMatch(x1.getValue(CPos("G14")), CValue(15.0)));
    assert(valueMatch(x1.getValue(CPos("H10")), CValue()));
    assert(valueMatch(x1.getValue(CPos("H11")), CValue()));
    assert(valueMatch(x1.getValue(CPos("H12")), CValue(25.0)));
    assert(valueMatch(x1.getValue(CPos("H13")), CValue(-22.0)));
    assert(valueMatch(x1.getValue(CPos("H14")), CValue(-22.0)));

    x1.copyRect(CPos("f13"), CPos("G13"), 2, 2);

    oss.clear();
    oss.str("");
    assert(x1.save(oss));
    data = oss.str();
    iss.clear();
    iss.str(data);
    assert(x1.load(iss));

    assert(valueMatch(x1.getValue(CPos("F13")), CValue(15.0)));
    assert(valueMatch(x1.getValue(CPos("F14")), CValue(15.0)));
    assert(valueMatch(x1.getValue(CPos("G13")), CValue(65.0)));
    assert(valueMatch(x1.getValue(CPos("G14")), CValue(65.0)));

    x1.copyRect(CPos("f12"), CPos("G11"), 2, 2);

    oss.clear();
    oss.str("");
    assert(x1.save(oss));
    data = oss.str();
    iss.clear();
    iss.str(data);
    assert(x1.load(iss));

    assert(valueMatch(x1.getValue(CPos("F12")), CValue(35.0)));
    assert(valueMatch(x1.getValue(CPos("F13")), CValue(35.0)));
    assert(valueMatch(x1.getValue(CPos("G13")), CValue(35.0)));
    assert(valueMatch(x1.getValue(CPos("G12")), CValue(25.0)));

    x1.copyRect(CPos("h14"), CPos("G13"), 2, 2);

    oss.clear();
    oss.str("");
    assert(x1.save(oss));
    data = oss.str();
    iss.clear();
    iss.str(data);
    assert(x1.load(iss));

    assert(valueMatch(x1.getValue(CPos("h14")), CValue(45.0)));
    assert(valueMatch(x1.getValue(CPos("h15")), CValue(-22.0)));
    assert(valueMatch(x1.getValue(CPos("i14")), CValue()));
    assert(valueMatch(x1.getValue(CPos("I15")), CValue()));

    assert(x1.setCell(CPos("j3"), "=$B4+10"));
    assert(x1.setCell(CPos("j4"), "=B$5 + 10"));
    assert(x1.setCell(CPos("k3"), "=$E$3   +  10"));
    assert(x1.setCell(CPos("k4"), "=E4 +   10"));
    assert(x1.setCell(CPos("k5"), "=F13 -   H$14"));
    assert(x1.setCell(CPos("l4"), "=$B6 +   B$6"));
    assert(x1.setCell(CPos("l5"), "=F12 + H14"));
    assert(valueMatch(x1.getValue(CPos("j3")), CValue(12554.0)));
    assert(valueMatch(x1.getValue(CPos("j4")), CValue(20468.0)));
    assert(valueMatch(x1.getValue(CPos("k3")), CValue(100.0)));
    assert(valueMatch(x1.getValue(CPos("k4")), CValue(110.0)));
    assert(valueMatch(x1.getValue(CPos("K5")), CValue(-10.0)));
    assert(valueMatch(x1.getValue(CPos("l4")), CValue(81832.0)));
    assert(valueMatch(x1.getValue(CPos("L5")), CValue(80.0)));

    x1.copyRect(CPos("j3"), CPos("k4"), 2, 2);

    oss.clear();
    oss.str("");
    assert(x1.save(oss));
    data = oss.str();
    iss.clear();
    iss.str(data);
    assert(x1.load(iss));

    assert(valueMatch(x1.getValue(CPos("j3")), CValue(50.0)));
    assert(valueMatch(x1.getValue(CPos("j4")), CValue()));
    assert(valueMatch(x1.getValue(CPos("k3")),
                      CValue("20458.000000raw text with any characters, including a quote \" or a newline\n")));
    assert(valueMatch(x1.getValue(CPos("k4")), CValue()));

    x1.copyRect(CPos("k5"), CPos("j3"), 2, 2);

    oss.clear();
    oss.str("");
    assert(x1.save(oss));
    data = oss.str();
    iss.clear();
    iss.str(data);
    assert(x1.load(iss));

    assert(valueMatch(x1.getValue(CPos("k5")), CValue()));
    assert(valueMatch(x1.getValue(CPos("k6")), CValue(-30.0)));
    assert(valueMatch(x1.getValue(CPos("l5")), CValue()));
    assert(valueMatch(x1.getValue(CPos("l6")), CValue(13.0)));

    assert(x1.setCell(CPos("l4"), "=$B4 +   B$4"));
    assert(x1.setCell(CPos("l5"), "=$B5 +   B5"));
    assert(valueMatch(x1.getValue(CPos("l4")), CValue(25088.0)));
    assert(valueMatch(x1.getValue(CPos("L5")), CValue(40916.0)));

    x1.copyRect(CPos("l5"), CPos("l4"), 1, 2);

    oss.clear();
    oss.str("");
    assert(x1.save(oss));
    data = oss.str();
    iss.clear();
    iss.str(data);
    assert(x1.load(iss));

    assert(valueMatch(x1.getValue(CPos("l4")), CValue(25088.0)));
    assert(valueMatch(x1.getValue(CPos("L5")), CValue(33002.0)));
    assert(valueMatch(x1.getValue(CPos("L6")), CValue(81832.0)));

    x1.copyRect(CPos("l4"), CPos("l5"), 1, 2);

    oss.clear();
    oss.str("");
    assert(x1.save(oss));
    data = oss.str();
    iss.clear();
    iss.str(data);
    assert(x1.load(iss));

    assert(valueMatch(x1.getValue(CPos("l4")), CValue(25088.0)));
    assert(valueMatch(x1.getValue(CPos("L5")), CValue(40916.0)));
    assert(valueMatch(x1.getValue(CPos("L6")), CValue(81832.0)));

    assert(x1.setCell(CPos("h4"), "=sum($D0:E$4)"));
    assert(valueMatch(x1.getValue(CPos("h4")), CValue(550.0)));

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
