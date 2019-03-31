#pragma once

#include "value.hh"


class Symbols
{
public:
    typedef Value *(Handler)(std::string symbol, Value *v);

    Symbols();
    void registerSymbol(std::string name, Handler h);
    Handler *get(std::string name);
};
