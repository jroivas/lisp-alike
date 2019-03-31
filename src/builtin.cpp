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
        v = v->cdr();
        if (v == nullptr)
            return new IntValue(a);
    }
    double b = a;
    while (true) {
        if (v->type() == Type::Int) b += toInt(v)->value();
        else if (v->type() == Type::Float) b += toFloat(v)->value();
        else PARSE_ERROR("Can sum only int and float!");
        v = v->cdr();
        if (v == nullptr)
            return new FloatValue(b);
    }

    return nullptr;
}

Value *Builtin::minus(Value *v)
{
    if (v == nullptr) return nullptr;
    long long int a = 0;
    bool first = true;
    while (v->type() == Type::Int) {
        a = first ? toInt(v)->value() : a - toInt(v)->value();
        v = v->cdr();
        if (v == nullptr)
            return new IntValue(a);
        first = false;
    }
    double b = a;
    while (true) {
        if (v->type() == Type::Int) b -= toInt(v)->value();
        else if (v->type() == Type::Float)
            b = first ? toFloat(v)->value() :
                b - toFloat(v)->value();
        else PARSE_ERROR("Can subtract only int and float!");
        v = v->cdr();
        if (v == nullptr)
            return new FloatValue(b);
        first = false;
    }

    return nullptr;
}
