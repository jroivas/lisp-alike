#include "value.hh"

std::string boolToString(bool v) {
    return v ? "true" : "false";
}

std::string listToString(Value *v) {
    std::string res = "(";
    while (v != nullptr) {
        if (!res.empty() && res[res.length() - 1] != '(')
            res += " ";
        res += v->toString();
        v = v->cdr();
    }
    res += ")";
    return res;
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
        default: return "<INVALID>";
    };
}
