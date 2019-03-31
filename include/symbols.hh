#pragma once

#include "value.hh"
#include <unordered_map>

class Symbols
{
public:
    typedef Value *(Handler)(Value *a, Value *b);

    Symbols() {}
    void registerSymbol(std::string name, Handler h);
    Handler *get(std::string name);

private:
    std::unordered_map<std::string, Symbols::Handler *>handlers;
};
