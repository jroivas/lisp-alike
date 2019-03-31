#include "symbols.hh"

void Symbols::registerSymbol(std::string name, Handler h)
{
    handlers[name] = h;
}

Symbols::Handler *Symbols::get(std::string name)
{
    auto res = handlers.find(name);
    if (res == handlers.end()) return nullptr;

    return res->second;
}
