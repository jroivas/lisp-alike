#pragma once

#include <string>
#include <unordered_map>
#include "value.hh"

class Env
{
public:
    Env() : next(nullptr) {}
    Env(Env *outer) : next(outer) {}

    void set(std::string key, Value *v);
    Value *get(std::string key);

private:
    Env *next;
    std::unordered_map<std::string, Value *> values;
};
