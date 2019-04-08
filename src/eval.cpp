#include "eval.hh"
#include "builtin.hh"

Eval::Eval(Symbols &s, Env &e) :
    symbols(s), env(e)
{
}

Value *Eval::eval(Parse &parse)
{
    return evalValue(parse.readForm());
}

Value *Eval::evalValue(Value *v)
{
    if (v == nullptr) return nullptr;

    switch (v->type()) {
        case Type::List: return evalList(toList(v));
        case Type::Vector: return evalVector(toVector(v));
        case Type::Symbol: return evalSymbol(toSymbol(v));
        case Type::Function:
            return evalFunction(toFunction(v), v->cdr());
        default: return v;
    }
}

Value *Eval::evalSymbol(SymbolValue *symbol)
{
    std::string val = symbol->value();
    auto symbolData = symbols.getSymbol(val);
    if (symbolData == nullptr) {
        Value *v = env.get(val);
        if (v != nullptr) return v;
        return symbol;
    }
    auto handler = symbolData->handler;

    Value *res = symbol->cdr();
    Value *b = symbol->cdr();
    while (b != nullptr && b->cdr() != nullptr) {
        b = b->cdr();
        res = handler(res, b, this, &env);
        if (!symbolData->recurse) break;
    }
    return res;
}

Value *Eval::evalList(ListValue *list)
{
    Value *v = list->value();
    if (v == nullptr) return list;

    if (v->type() == Type::Symbol)
        return evalSymbol(toSymbol(v));

    Value *v2 = evalValue(v);
    if (v2 != nullptr && v2->type() == Type::Function)
        return evalFunction(toFunction(v2), v->cdr());

    ERROR("Not applicable: " + v->toString());
}

Value *Eval::evalVector(VectorValue *list)
{
    Value *v = list->value();
    Value *res = nullptr;
    while (v != nullptr) {
        Value *next = v->cdr();
        Value *tmp = evalValue(v->clone());
        if (res == nullptr) res = tmp;
        else res->addLast(tmp);
        v = next;
    }

    return new VectorValue(res);
}

Value *Eval::evalFunction(FunctionValue *function, Value *n)
{
    Env *fnEnv = new Env();
    Value *bind = function->params();
    Value *v = iterInit(bind);
    while (v != nullptr) {
        if (v->type() != Type::Symbol)
            ERROR("Expected symbol!")
        std::string s = toSymbol(v)->value();
        fnEnv->set(s, n);
        if (n != nullptr) n = n->cdr();
        v = iterNext(v);
    }

    Value *body = function->value()->clone();
    bind = function->params();
    v = iterInit(bind);
    while (v != nullptr) {
        body->addLast(v->clone());
        v = iterNext(v);
    }

    Eval ev(symbols, *fnEnv);
    return ev.evalValue(body);
}
