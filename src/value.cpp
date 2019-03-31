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

