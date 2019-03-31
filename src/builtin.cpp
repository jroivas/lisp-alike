#include "builtin.hh"

Builtin::Builtin(Symbols &s) : symbols(s)
{
    init();
}

void Builtin::init()
{
    symbols.registerSymbol("+", this->plus);
}

Value *Builtin::plus(Value *v)
{
    if (v == nullptr) return nullptr;
    long long int a = 0;
    while(v->type() == Type::Int) {
        a += toInt(v)->value();
        if (v->cdr() == nullptr)
            return new IntValue(a);
        v = v->cdr();
    }
    double b = a;
    while(true) {
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
