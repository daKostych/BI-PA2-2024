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
    builderStack.emplace(new OperatorComp(std::move(lhs), std::move(rhs), "!="));
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
//void valRange(std::string val) {} // todo
//======================================================================================================================
//void funcCall(std::string fnName, int paramCount) {} // todo
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