#include "builder.h"
#include <variant>
#include <memory>

using CValue = std::variant<std::monostate, double, std::string>;
using ANode = std::unique_ptr<Node>;
//======================================================================================================================
void Builder::opAdd()
{
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();
    builderStack.emplace(new OperatorAdd(std::move(lhs), std::move(rhs)));
}
//======================================================================================================================
void Builder::opSub()
{
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();
    builderStack.emplace(new OperatorSub(std::move(lhs), std::move(rhs)));
}
//======================================================================================================================
void Builder::opMul()
{
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();
    builderStack.emplace(new OperatorMul(std::move(lhs), std::move(rhs)));
}
//======================================================================================================================
void Builder::opDiv()
{
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();
    builderStack.emplace(new OperatorDiv(std::move(lhs), std::move(rhs)));
}
//======================================================================================================================
void Builder::opPow()
{
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();
    builderStack.emplace(new OperatorPow(std::move(lhs), std::move(rhs)));
}
//======================================================================================================================
void Builder::opNeg()
{
    ANode node = std::move(builderStack.top());
    builderStack.pop();
    builderStack.emplace(new OperatorNeg(std::move(node)));
}
//======================================================================================================================
void Builder::opEq()
{
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();
    builderStack.emplace(new OperatorComp(std::move(lhs), std::move(rhs), "=="));
}
//======================================================================================================================
void Builder::opNe()
{
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();
    builderStack.emplace(new OperatorComp(std::move(lhs), std::move(rhs), "<>"));
}
//======================================================================================================================
void Builder::opLt()
{
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();
    builderStack.emplace(new OperatorComp(std::move(lhs), std::move(rhs), "<"));
}
//======================================================================================================================
void Builder::opLe()
{
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();
    builderStack.emplace(new OperatorComp(std::move(lhs), std::move(rhs), "<="));
}
//======================================================================================================================
void Builder::opGt()
{
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();
    builderStack.emplace(new OperatorComp(std::move(lhs), std::move(rhs), ">"));
}
//======================================================================================================================
void Builder::opGe()
{
    ANode rhs = std::move(builderStack.top());
    builderStack.pop();
    ANode lhs = std::move(builderStack.top());
    builderStack.pop();
    builderStack.emplace(new OperatorComp(std::move(lhs), std::move(rhs), ">="));
}
//======================================================================================================================
void Builder::valNumber(double val) { builderStack.emplace(new Operand(val)); }
//======================================================================================================================
void Builder::valString(std::string val) { builderStack.emplace(new Operand(val)); }
//======================================================================================================================
void Builder::valReference(std::string val)
{
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
void Builder::valRange(std::string val)
{
    size_t colonPos = val.find(':');
    bool upperLeftRowAb = val.substr(1, colonPos - 1).find('$') != string::npos;
    bool upperLeftColumnAb = val[0] == '$';
    bool lowerRightRowAb = val.substr(colonPos + 2).find('$') != string::npos;
    bool lowerRightColumnAb = val[colonPos + 1] == '$';
    builderStack.emplace(new RangeOperand(CPos(removeDollars(val.substr(0, colonPos))),
                                             CPos(removeDollars(val.substr(colonPos + 1))),
                                             upperLeftColumnAb, upperLeftRowAb,
                                             lowerRightColumnAb, lowerRightRowAb));
}
//======================================================================================================================
void Builder::funcCall(std::string fnName, int paramCount)
{
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
string Builder::removeDollars(const string & str)
{
    string result;
    for (char c : str)
        if (c != '$')
            result += c;
    return result;
}
//======================================================================================================================