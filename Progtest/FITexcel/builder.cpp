#include "builder.h"
#include <variant>
#include <memory>

using CValue = variant<monostate, double, string>;
using ANode = unique_ptr<Node>;
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