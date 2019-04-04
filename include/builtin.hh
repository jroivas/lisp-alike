#pragma once

#include <string>
#include "value.hh"
#include "symbols.hh"
#include "env.hh"

class Builtin
{
public:
    Builtin(Symbols &s);
    void init();

private:
    Symbols &symbols;
    static Value *plus(Value *a, Value *b, Env *n);
    static Value *minus(Value *a, Value *b, Env *n);
    static Value *mul(Value *a, Value *b, Env *n);
    static Value *div(Value *a, Value *b, Env *n);
    static Value *def(Value *a, Value *b, Env *n);
    static Value *let_star(Value *a, Value *b, Env *n);
};
