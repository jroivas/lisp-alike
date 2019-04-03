#include "eval.hh"
#include "builtin.hh"

Eval::Eval(Parse &p, Symbols &s, Env &e) :
    parse(p), symbols(s), env(e)
{
}

Value *Eval::eval()
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
    auto h = symbols.get(val);
    if (h == nullptr)
        return new StringValue("ERROR: Unknown symbol: " + val);

    Value *res = evalValue(symbol->cdr());
    Value *b = symbol->cdr();
    while (b != nullptr && b->cdr() != nullptr) {
        b = evalValue(b->cdr());
        res = h(res, b, &env);
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
