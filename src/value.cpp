#include "value.hh"

std::string boolToString(bool v) {
    return v ? "true" : "false";
}

std::string listyToString(Value *v, char s, char e) {
    std::string res;
    res += s;
    while (v != nullptr) {
        if (!res.empty() && res[res.length() - 1] != s)
            res += " ";
        res += v->toString();
        v = v->cdr();
    }
    return res + e;
}

std::string typeStr(Type t)
{
    switch (t) {
        case Type::None: return "None";
        case Type::String: return "String";
        case Type::Int: return "Int";
        case Type::Float: return "Float";
        case Type::Symbol: return "Symbol";
        case Type::Nil: return "Nil";
        case Type::Bool: return "Bool";
        case Type::List: return "List";
        case Type::Function: return "Function";
        default: return "<INVALID>";
    };
}

bool listCompartor(const Value *a, const Value *b)
{
    //FIXME
    return a == b;
}
