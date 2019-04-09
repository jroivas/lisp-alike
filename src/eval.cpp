#include "eval.hh"
#include "builtin.hh"

Value *evalLine(Symbols &s, Env &env, std::string line)
{
    Tokenize tok(line);
    Parse p(tok);
    Eval e(s, env);
    return e.eval(p);
}

Value *evalLine(std::string l)
{
    Symbols s;
    Builtin b(s);
    Env env;

    return evalLine(s, env, l);
}

Eval::Eval(Symbols &s, Env &e) :
    symbols(s), env(e)
{
}

Value *Eval::eval(Parse &parse)
{
    return evalValue(parse.readForm());
}

Value *Eval::evalValue(Value *v)
{
    if (v == nullptr) return nullptr;

    switch (v->type()) {
        case Type::List: return evalList(toList(v));
        case Type::Vector: return evalVector(toVector(v));
        case Type::Symbol: return evalSymbol(toSymbol(v));
        case Type::Function:
            return evalFunction(toFunction(v), v->cdr());
        default: return v;
    }
}

Value *Eval::evalSymbol(SymbolValue *symbol)
{
    std::string val = symbol->value();
    auto symbolData = symbols.getSymbol(val);
    if (symbolData == nullptr) {
        Value *v = env.get(val);
        if (v == nullptr)
            return symbol;
        if (v->type() != Type::Function)
            return v;
        return evalFunction(toFunction(v), symbol->cdr());
    }
    auto handler = symbolData->handler;

    Value *res = symbol->cdr();
    Value *b = symbol->cdr();
    // Needs to be evaluated at least once
    do {
        if (b != nullptr)
            b = b->cdr();
        res = handler(res, b, this, &env);
    } while (b != nullptr && symbolData->recurse);

    return res;
}

Value *Eval::evalList(ListValue *list)
{
    Value *v = list->value();
    if (v == nullptr) return list;

    if (v->type() == Type::Symbol)
        return evalSymbol(toSymbol(v));

    Value *v2 = evalValue(v);
    if (v2 != nullptr && v2->type() == Type::Function)
        return evalFunction(toFunction(v2), v->cdr());

    ERROR("Not applicable: " + v->toString());
}

Value *Eval::evalVector(VectorValue *list)
{
    Value *v = list->value();
    Value *res = nullptr;
    while (v != nullptr) {
        Value *next = v->cdr();
        Value *tmp = evalValue(v->clone());
        if (res == nullptr) res = tmp;
        else res->addLast(tmp);
        v = next;
    }

    return new VectorValue(res);
}

Value *Eval::evalFunction(FunctionValue *function, Value *n)
{
    Env *fnEnv = Env::bind(iterInit(function->params()), n);
    Value *body = function->value()->clone();

    Eval ev(symbols, *fnEnv);
    return ev.evalValue(body);
}
