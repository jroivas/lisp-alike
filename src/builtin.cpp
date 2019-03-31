#include "builtin.hh"

Builtin::Builtin(Symbols &s) : symbols(s)
{
    init();
}

void Builtin::init()
{
    symbols.registerSymbol("+", this->plus);
    symbols.registerSymbol("-", this->minus);
}

Value *Builtin::plus(Value *v)
{
    if (v == nullptr) return nullptr;
    long long int a = 0;
    while (v->type() == Type::Int) {
        a += toInt(v)->value();
        if (v->cdr() == nullptr)
            return new IntValue(a);
        v = v->cdr();
    }
    double b = a;
    while (true) {
        if (v->type() == Type::Int) {
            b += toInt(v)->value();
        } else if (v->type() == Type::Float) {
            b += toFloat(v)->value();
        } else {
            PARSE_ERROR("Can sum only int and float!");
        }
        if (v->cdr() == nullptr)
            return new FloatValue(b);
        v = v->cdr();
    }

    return nullptr;
}

Value *Builtin::minus(Value *v)
{
    if (v == nullptr) return nullptr;
    long long int a = 0;
    bool first = true;
    while (v->type() == Type::Int) {
        if (first) a = toInt(v)->value();
        else a -= toInt(v)->value();
        if (v->cdr() == nullptr)
            return new IntValue(a);
        v = v->cdr();
        first = false;
    }
    double b = a;
    while (true) {
        if (v->type() == Type::Int) {
            b -= toInt(v)->value();
        } else if (v->type() == Type::Float) {
            if (first) b = toFloat(v)->value();
            else b -= toFloat(v)->value();
        } else {
            PARSE_ERROR("Can subtract only int and float!");
        }
        if (v->cdr() == nullptr)
            return new FloatValue(b);
        v = v->cdr();
        first = false;
    }

    return nullptr;
}
