#pragma once

#include "parse.hh"
#include "value.hh"

class Eval
{
public:
    Eval(Parse &p) : parse(p) {}
    Value *eval();

private:
    Parse &parse;
};
