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

protected:
    Value(Type t) : valueType(t), next(nullptr) {}

private:
    Type valueType;

    Value *next;
};

#define ValueDef(x, y, t)\
class x ## Value : public Value\
{\
public:\
    x ## Value(t v) : Value(y), _value(v) {}\
    t value() { return _value; } \
private:\
    t _value;\
};\
x ## Value* to##x(Value *val) {\
    if (val->type() != y) throw "Type mismatch!";\
    return static_cast<x ## Value*>(val); \
}

ValueDef(String, Type::String, std::string)
ValueDef(Symbol, Type::Symbol, std::string)
ValueDef(Bool, Type::Bool, bool)
ValueDef(Int, Type::Int, long long int)
ValueDef(List, Type::List, Value *)

class NilValue : public Value
{
public:
    NilValue() : Value(Type::Nil) {}
    Value *value() { return nullptr; }
};
NilValue *toNil(Value *val) {
    if (val->type() != Type::Nil) throw "Type mismatch!";
    return static_cast<NilValue*>(val);
}
