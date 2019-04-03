#pragma once

#include <unordered_map>
#include "value.hh"
#include "env.hh"

class Symbols
{
public:
    typedef Value *(Handler)(Value *a, Value *b, Env *e);

    Symbols() {}
    void registerSymbol(std::string name, Handler h);
    Handler *get(std::string name);

private:
    std::unordered_map<std::string, Symbols::Handler *>handlers;
};
