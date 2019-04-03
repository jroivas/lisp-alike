#pragma once

#include "value.hh"
#include "parse.hh"
#include "symbols.hh"
#include "env.hh"

class Eval
{
public:
    Eval(Parse &p, Symbols &s, Env &e);
    Value *eval();

private:
    Value *evalValue(Value *v);
    Value *evalList(ListValue *list);
    Value *evalSymbol(SymbolValue *symbol);

    Parse &parse;
    Symbols &symbols;
    Env &env;
};
