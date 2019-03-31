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
    static Value *plus(Value *v);
};
