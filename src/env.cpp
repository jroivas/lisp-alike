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

    //Value *body = function->value()->clone();
    /*
    Value *restList = nullptr;
    bind = params;
    v = bind;
    while (v != nullptr) {
        //body->addLast(v->clone());
        v = iterNext(v);
    }
    */
    return fnEnv;
#if 0
    Env *tmp = new Env();
    Value *v = bind;
    bool restList = false;
    std::string restName;
    Value *restListItem = nullptr;
    std::cout << "---\n";
    while (v != nullptr) {
        if (v->type() != Type::Symbol)
            ERROR("Expected symbol!")
        std::string s = toSymbol(v)->value();
        std::cout << "SS " << s<<"\n";
        if (restList) {
            std::cout << "S1\n";
            if (restListItem)
                ERROR("Expected only one item after &");
            restName = s;
            restListItem = get(s)->clone();
        } else if (s == "&") {
            std::cout << "S2\n";
            restList = true;
        } else {
            std::cout << "S3\n";
            tmp->set(s, get(s));
        }
        v = iterNext(v);
    }
    if (restList)
        tmp->set(restName, new ListValue(restListItem));
    return tmp;
#endif
}
