#include "symbols.hh"

#include <unordered_map>

static std::unordered_map<std::string, Symbols::Handler *>__handlers;

Symbols::Symbols()
{
}

void Symbols::registerSymbol(std::string name, Handler h)
{
    __handlers[name] = h;
}

Symbols::Handler *Symbols::get(std::string name)
{
    return nullptr;
}
