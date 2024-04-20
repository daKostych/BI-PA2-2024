#pragma once

#include "expression.h"
#include "node.h"
#include <stack>

class Builder : public CExprBuilder
{
public:
    Builder() : CExprBuilder() {};

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
    //void valRange(std::string val) override;
    //void funcCall(std::string fnName, int paramCount) override;

    stack<ANode> builderStack;

private:
    static string removeDollars(const string & str);
};