#include "builtin.hh"
#include "parse.hh"
#include "eval.hh"

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
    symbols.registerSymbol("let*", this->let_star);
}

double getDoubleNumber(Value *v)
{
    if (v->type() == Type::Int) return toInt(v)->value();
    else if (v->type() == Type::Float) return toFloat(v)->value();
    PARSE_ERROR("Can sum only int and float!");
    return 0;
}

Value *Builtin::plus(Value *a, Value *b, Env *n)
{
    if (a == nullptr && b == nullptr) return nullptr;
    if (a == nullptr) return b;
    if (b == nullptr) return a;

    Symbols s;
    Builtin bin(s);
    Eval e(s, *n);

    a = e.evalValue(a);
    b = e.evalValue(b);

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

static bool evalListItem(Value *item, Eval *e, Env *evalEnv)
{
    if (item->type() != Type::Symbol) return false;

    std::string key = toSymbol(item)->value();
    Value *v = e->evalValue(item->cdr());
    evalEnv->set(key, v);
    return true;
}

static bool evalList(Value *a, Eval *e, Env *evalEnv)
{
    if (a->type() != Type::List) return false;

    Value *item = toList(a)->value();
    if (item->type() != Type::Symbol) return false;

    return evalListItem(item, e, evalEnv);
}

Value *Builtin::let_star(Value *a, Value *b, Env *n)
{
    if (a->type() != Type::List) {
        // Force wrap to list
        a = new ListValue(a);
    }

    Env evalEnv(n);
    Symbols s;
    Builtin bin(s);
    Eval e(s, evalEnv);

    if (evalList(a, &e, &evalEnv))
        return e.evalValue(b);

    Value *item = toList(a)->value();
    while (item != nullptr) {
        if (item->type() != Type::List) break;
        evalList(item, &e, &evalEnv);
        item = item->cdr();
    }

    return e.evalValue(b);
}
