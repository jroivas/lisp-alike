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

Value *Env::handleSymbol(Value *v)
{
    if (v->type() != Type::Symbol) return v;
    Value *tmp = get(toSymbol(v)->value());
    if (tmp != nullptr) return tmp;
    return v;
}

Env *Env::bind(Value *bind, Value *params)
{
    Env *fnEnv = new Env();
    Value *v = bind;
    Value *n = params;
    std::string restName;
    bool rest = false;
    Value *restValue = nullptr;
    while (v != nullptr) {
        if (v->type() != Type::Symbol)
            ERROR("Expected symbol!")
        std::string s = toSymbol(v)->value();
        if (rest) {
            restName = s;
            if (v->cdr() != nullptr)
                ERROR("Expected only one item after &, got: " + s);
            break;
        } else if (s == "&") {
            rest = true;
        } else {
            fnEnv->set(s, n);
        }
        if (n == nullptr) ERROR("Not enought parameters");
        if (!rest) n = n->cdr();
        v = v->cdr();
    }

    if (rest && restName.empty())
        ERROR("Expected one item after &");

    while (n != nullptr) {
        if (restValue == nullptr)
            restValue = n->clone();
        else
            restValue->addLast(n->clone());
        n = n->cdr();
    }
    fnEnv->set(restName, new ListValue(restValue));

    return fnEnv;
}
