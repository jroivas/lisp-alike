#include "eval.hh"
#include "builtin.hh"

Eval::Eval(Parse &p, Symbols &s) : parse(p), symbols(s)
{
}

Value *Eval::eval()
{
    Value *v = parse.readForm();
    if (v == nullptr) return nullptr;

    if (v->type() == Type::List)
        return evalList(toList(v));

    return v;
}

Value *Eval::evalSymbol(SymbolValue *symbol)
{
    std::string val = symbol->value();
    auto h = symbols.get(val);
    if (h == nullptr) return nullptr;

    return h(symbol->cdr());
}

Value *Eval::evalList(ListValue *list)
{
    Value *v = list->value();
    if (v == nullptr) return list;
    if (v->type() == Type::Symbol)
        return evalSymbol(toSymbol(v));

    return v;
}
