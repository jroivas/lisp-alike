#include "eval.hh"

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
    return nullptr;
}

Value *Eval::evalList(ListValue *list)
{
    Value *v = list->value();
    if (v == nullptr) return list;
    if (v->type() == Type::Symbol)
        return evalSymbol(toSymbol(v));

    return v;
}
