#pragma once

#include "tokenize.hh"
#include "value.hh"

class Parse
{
public:
    Parse(Tokenize &t) : tokenize(t) {}
    Value *readForm();
    Value *readList(std::string endMark);

private:
    Tokenize &tokenize;
};
