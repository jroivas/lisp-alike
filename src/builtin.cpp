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
    symbols.registerSymbol("def!", this->def, false);
    symbols.registerSymbol("let*", this->let_star, false);
    symbols.registerSymbol("if", this->if_kw, false);
    symbols.registerSymbol("do", this->do_kw, false);
    symbols.registerSymbol("fn*", this->fn_star, false);
    symbols.registerSymbol("list", this->list, false);
    symbols.registerSymbol("list?", this->list_is, false);
    symbols.registerSymbol("empty?", this->empty_is, false);
    symbols.registerSymbol("count", this->count, false);
    symbols.registerSymbol("=", this->equals);
}

double getDoubleNumber(Value *v)
{
    if (v->type() == Type::Int) return toInt(v)->value();
    else if (v->type() == Type::Float) return toFloat(v)->value();
    PARSE_ERROR("Can sum only int and float!");
    return 0;
}

Value *Builtin::plus(Value *a, Value *b, Eval *ev, Env *n)
{
    a = ev->evalValue(a);
    b = ev->evalValue(b);

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

Value *Builtin::minus(Value *a, Value *b, Eval *ev, Env *e)
{
    a = ev->evalValue(a);
    b = ev->evalValue(b);

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

Value *Builtin::mul(Value *a, Value *b, Eval *ev, Env *e)
{
    a = ev->evalValue(a);
    b = ev->evalValue(b);

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

Value *Builtin::div(Value *a, Value *b, Eval *ev, Env *e)
{
    a = ev->evalValue(a);
    b = ev->evalValue(b);

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

Value *Builtin::def(Value *a, Value *b, Eval *ev, Env *e)
{
    a = ev->evalValue(a);
    b = ev->evalValue(b);

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

Value *Builtin::let_star(Value *a, Value *b, Eval *ev, Env *n)
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

bool isTruth(Value *v)
{
    switch (v->type()) {
        case Type::Bool: return toBool(v)->value();
        case Type::Nil: return false;
        default: return true;
    }
}

Value *Builtin::if_kw(Value *a, Value *b, Eval *ev, Env *n)
{
    a = ev->evalValue(a);

    if (isTruth(a)) return ev->evalValue(b);

    return ev->evalValue(b->cdr());
}

Value *Builtin::do_kw(Value *a, Value *b, Eval *ev, Env *n)
{
    Value *res = ev->evalValue(a);

    while (b != nullptr) {
        Value *next = b->cdr();
        res = ev->evalValue(b);
        b = next;
    }
    return res;
}

Value *Builtin::fn_star(Value *a, Value *b, Eval *ev, Env *n)
{
    return new FunctionValue(a, b);
}

Value *Builtin::list(Value *a, Value *b, Eval *ev, Env *n)
{
    if (a == nullptr) return new ListValue(nullptr);

    Value *res = ev->evalValue(a->clone());
    while (b != nullptr) {
        res->addLast(ev->evalValue(b->clone()));
        b = b->cdr();
    }

    return new ListValue(res);
}

Value *Builtin::list_is(Value *a, Value *b, Eval *ev, Env *n)
{
    a = ev->evalValue(a);
    return new BoolValue(a != nullptr && a->type() == Type::List);
}

Value *Builtin::empty_is(Value *a, Value *b, Eval *ev, Env *n)
{
    a = ev->evalValue(a);
    if (a == nullptr || (a->type() != Type::List &&
            a->type() != Type::Vector))
        ERROR("Expected list kind of type to empty?");

    Value *v = a->type() == Type::List
        ? toList(a)->value()
        : toVector(a)->value();
    return new BoolValue(v == nullptr);
}

Value *Builtin::count(Value *a, Value *b, Eval *ev, Env *n)
{
    a = ev->evalValue(a);
    if (a == nullptr || (a->type() != Type::List &&
            a->type() != Type::Vector))
        ERROR("Expected list kind of type to empty?");

    Value *v = a->type() == Type::List
        ? toList(a)->value()
        : toVector(a)->value();

    long long int cnt = 0;
    while (v != nullptr) {
        ++cnt;
        v = v->cdr();
    }

    return new IntValue(cnt);
}

Value *Builtin::equals(Value *a, Value *b, Eval *ev, Env *n)
{
    a = ev->evalValue(a);
    b = ev->evalValue(b);

    bool r = a->equals(b);
    return new BoolValue(r);
}
