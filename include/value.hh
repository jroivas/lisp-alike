#pragma once

enum class Type {
    None,
    String,
    Int,
    Symbol,
    Nil,
    Bool,
    List
};

class Value
{
public:
    Value() : valueType(Type::None), next(nullptr) {}
    virtual ~Value() {}

    Type type() const {
        return valueType;
    }
    Value *car() {
        return this;
    }
    Value *cdr() {
        return next;
    }
    void addLast(Value *v) {
        Value *p = this;
        while (p->next != nullptr) p = p->next;
        p->next = v;
    }
    void setNext(Value *v) {
        next = v;
    }

    virtual std::string toString() const {
        return "<INVALID>";
    }

protected:
    Value(Type t) : valueType(t), next(nullptr) {}

private:
    Type valueType;

    Value *next;
};

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

#define ValueDef(x, y, t, s)\
class x ## Value : public Value\
{\
public:\
    x ## Value(t v) : Value(y), _value(v) {}\
    t value() { return _value; } \
    virtual std::string toString() const {\
        return s(_value);\
    }\
private:\
    t _value;\
};\
x ## Value* to##x(Value *val) {\
    if (val->type() != y) throw "Type mismatch!";\
    return static_cast<x ## Value*>(val); \
}

ValueDef(String, Type::String, std::string, )
ValueDef(Symbol, Type::Symbol, std::string, )
ValueDef(Bool, Type::Bool, bool, boolToString)
ValueDef(Int, Type::Int, long long int, std::to_string)
ValueDef(List, Type::List, Value *, listToString)

class NilValue : public Value
{
public:
    NilValue() : Value(Type::Nil) {}
    Value *value() { return nullptr; }
    std::string toString() const { return "nil"; }
};
NilValue *toNil(Value *val) {
    if (val->type() != Type::Nil) throw "Type mismatch!";
    return static_cast<NilValue*>(val);
}
