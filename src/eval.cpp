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

    if (v->type() == Type::List)
        return evalList(toList(v));

    if (v->type() == Type::Symbol)
        return evalSymbol(toSymbol(v));

    return v;
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

    return v;
}
