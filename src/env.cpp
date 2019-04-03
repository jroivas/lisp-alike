#include "env.hh"

void Env::set(std::string key, Value *v)
{
    values[key] = v;
}

Value *Env::get(std::string key)
{
    auto item = values.find(key);
    if (item == values.end()) {
        if (next != nullptr) return next->get(key);
        return nullptr;
    }

    return item->second;
}
