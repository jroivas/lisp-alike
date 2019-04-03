#include "builtin.hh"

Builtin::Builtin(Symbols &s) : symbols(s)
{
    init();
}

void Builtin::init()
{
    symbols.registerSymbol("+", this->plus);
    symbols.registerSymbol("-", this->minus);
    symbols.registerSymbol("*", this->mul);
    symbols.registerSymbol("/", this->div);
    symbols.registerSymbol("def!", this->def);
}

double getDoubleNumber(Value *v)
{
    if (v->type() == Type::Int) return toInt(v)->value();
    else if (v->type() == Type::Float) return toFloat(v)->value();
    PARSE_ERROR("Can sum only int and float!");
    return 0;
}

Value *Builtin::plus(Value *a, Value *b, Env *e)
{
    if (a == nullptr && b == nullptr) return nullptr;
    if (a == nullptr) return b;
    if (b == nullptr) return a;

    if (a->type() == Type::Float ||
        b->type() == Type::Float) {
        return new FloatValue(getDoubleNumber(a) +
                getDoubleNumber(b));
    }
    return new IntValue(toInt(a)->value() + toInt(b)->value());
}

Value *Builtin::minus(Value *a, Value *b, Env *e)
{
    if (a == nullptr && b == nullptr) return nullptr;
    if (a == nullptr) return b;
    if (b == nullptr) return a;

    if (a->type() == Type::Float ||
        b->type() == Type::Float) {
        return new FloatValue(getDoubleNumber(a) -
                getDoubleNumber(b));
    }
    return new IntValue(toInt(a)->value() - toInt(b)->value());
}

Value *Builtin::mul(Value *a, Value *b, Env *e)
{
    if (a == nullptr && b == nullptr) return nullptr;
    if (a == nullptr) return b;
    if (b == nullptr) return a;

    if (a->type() == Type::Float ||
        b->type() == Type::Float) {
        return new FloatValue(getDoubleNumber(a) *
                getDoubleNumber(b));
    }
    return new IntValue(toInt(a)->value() * toInt(b)->value());
}

Value *Builtin::div(Value *a, Value *b, Env *e)
{
    if (a == nullptr && b == nullptr) return nullptr;
    if (a == nullptr) return b;
    if (b == nullptr) return a;

    if (a->type() == Type::Float ||
        b->type() == Type::Float) {
        double div = getDoubleNumber(b);
        if (div == 0.0) ERROR("Divide by zero");
        return new FloatValue(getDoubleNumber(a) / div);
    }
    long long int div = toInt(b)->value();
    if (div == 0) ERROR("Divide by zero");
    return new IntValue(toInt(a)->value() / div);
}

Value *Builtin::def(Value *a, Value *b, Env *e)
{
    if (a == nullptr) return nullptr;
    if (a->type() != Type::Symbol) return nullptr;

    e->set(toSymbol(a)->value(), b);
    return b;
}
