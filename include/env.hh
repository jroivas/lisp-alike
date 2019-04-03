#pragma once

#include <string>
#include <unordered_map>
#include "value.hh"

class Env
{
public:
    Env() {}

    void set(std::string key, Value *v);
    Value *get(std::string key);

private:
    std::unordered_map<std::string, Value *> values;
};
