#ifndef __TRAINER__

#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#endif // __TRAINER__

using T = double;

class Expr {
public:
    virtual ~Expr() = default;

    virtual void print(std::ostream &os) const = 0;

    virtual T eval() const = 0;

    friend std::ostream &operator<<(std::ostream &os, const Expr &expr) {
        expr.print(os);
        return os;
    }

    friend std::partial_ordering operator<=>(const Expr &lhs, const Expr &rhs) {
        return lhs.eval() <=> rhs.eval();
    }

    friend bool operator==(const Expr &lhs, const Expr &rhs) {
        return lhs.eval() == rhs.eval();
    }
};

using AExpr = std::shared_ptr<Expr>;

class Literal : public Expr
{
public:
    Literal(T x) { literal = x; };
    T eval() const override { return literal; };

    void print(std::ostream &os) const override {
        os << literal;
    }

private:
    T literal;
};

class Addition : public Expr {
public:
    Addition(AExpr lhs, AExpr rhs) : m_Lhs(std::move(lhs)), m_Rhs(std::move(rhs)) {}

    T eval() const override {
        return m_Lhs->eval() + m_Rhs->eval();
    }

    void print(std::ostream &os) const override {
        os << "(";
        m_Lhs->print(os);
        os << " + ";
        m_Rhs->print(os);
        os << ")";
    }

private:
    AExpr m_Lhs;
    AExpr m_Rhs;
};

class Multiplication : public Expr
{
public:
    Multiplication(AExpr lhs, AExpr rhs) : m_Lhs(std::move(lhs)), m_Rhs(std::move(rhs)) {};

    T eval() const override { return m_Lhs->eval() * m_Rhs->eval(); };

    void print(std::ostream &os) const override {
        os << "(";
        m_Lhs->print(os);
        os << " * ";
        m_Rhs->print(os);
        os << ")";
    }

private:
    AExpr m_Lhs;
    AExpr m_Rhs;
};

class Division : public Expr
{
public:
    Division(AExpr lhs, AExpr rhs) : m_Lhs(std::move(lhs)), m_Rhs(std::move(rhs)) {};

    T eval() const override { return m_Lhs->eval() / m_Rhs->eval(); };

    void print(std::ostream &os) const override {
        os << "(";
        m_Lhs->print(os);
        os << " / ";
        m_Rhs->print(os);
        os << ")";
    }

private:
    AExpr m_Lhs;
    AExpr m_Rhs;
};

class Subtraction : public Expr
{
public:
    Subtraction(AExpr lhs, AExpr rhs) : m_Lhs(std::move(lhs)), m_Rhs(std::move(rhs)) {};

    T eval() const override { return m_Lhs->eval() - m_Rhs->eval(); };

    void print(std::ostream &os) const override {
        os << "(";
        m_Lhs->print(os);
        os << " - ";
        m_Rhs->print(os);
        os << ")";
    }

private:
    AExpr m_Lhs;
    AExpr m_Rhs;
};

#ifndef __TRAINER__

AExpr lit(T val) { return std::make_shared<Literal>(val); }

AExpr add(AExpr lhs, AExpr rhs) { return std::make_shared<Addition>(std::move(lhs), std::move(rhs)); }

AExpr sub(AExpr lhs, AExpr rhs) { return std::make_shared<Subtraction>(std::move(lhs), std::move(rhs)); }

AExpr mul(AExpr lhs, AExpr rhs) { return std::make_shared<Multiplication>(std::move(lhs), std::move(rhs)); }

AExpr div(AExpr lhs, AExpr rhs) { return std::make_shared<Division>(std::move(lhs), std::move(rhs)); }


void assertPrint(const AExpr &e, const std::string &expected) {
    std::stringstream ss;
    ss << *e;
    assert(ss.str() == expected);
}

int main() {
    AExpr six1 = add(lit(1), add(lit(2), lit(3)));
    AExpr six2 = sub(lit(7), lit(1));
    AExpr six3 = mul(lit(1), mul(lit(2), lit(3)));
    AExpr seven1 = div(add(lit(3), add(lit(2), mul(lit(3), lit(3)))), sub(lit(-1), lit(-3)));
    AExpr five1 = lit(5);

    assert(six1->eval() == 6);
    assert(six2->eval() == 6);
    assert(six3->eval() == 6);
    assert(five1->eval() == 5);
    assert(seven1->eval() == 7);

    assert(*six1 == *six2);
    assert(*six2 == *six3);
    assert(*six3 <= *six2);
    assert(*six3 >= *six2);
    assert(*five1 != *six2);
    assert(*five1 < *six1);
    assert(*seven1 > *five1);

    assertPrint(six1, "(1 + (2 + 3))");
    assertPrint(six2, "(7 - 1)");
    assertPrint(six3, "(1 * (2 * 3))");
    assertPrint(seven1, "((3 + (2 + (3 * 3))) / (-1 - -3))");
    assertPrint(five1, "5");
}

#endif // __TRAINER__
