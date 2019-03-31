#include "symbols.hh"

#include <unordered_map>

static std::unordered_map<std::string, Symbols::Handler *>__handlers;

void Symbols::registerSymbol(std::string name, Handler h)
{
    __handlers[name] = h;
}

Symbols::Handler *Symbols::get(std::string name)
{
    auto res = __handlers.find(name);
    if (res == __handlers.end()) return nullptr;

    return res->second;
}
