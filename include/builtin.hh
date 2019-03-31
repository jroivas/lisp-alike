#pragma once

#include <string>
#include "value.hh"
#include "symbols.hh"

class Builtin
{
public:
    Builtin(Symbols &s);
    void init();

private:
    Symbols &symbols;
    static Value *plus(Value *a, Value *b);
    static Value *minus(Value *a, Value *b);
    static Value *mul(Value *a, Value *b);
    static Value *div(Value *a, Value *b);
};
