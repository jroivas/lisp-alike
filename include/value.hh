#pragma once

#include <iostream>
#include <string>
#include "errors.hh"

enum class Type {
    None,
    String,
    Int,
    Float,
    Symbol,
    Nil,
    Bool,
    List,
    Vector,
    Function
};

class Value
{
public:
    Value() : valueType(Type::None), next(nullptr) {}
    virtual ~Value() {}

    Type type() const {
        return valueType;
    }
    const Value *car() const {
        return this;
    }
    Value *cdr() const {
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
    virtual Value* clone() const {
        return new Value(valueType);
    }

protected:
    Value(Type t) : valueType(t), next(nullptr) {}

private:
    Type valueType;

    Value *next;
};

std::string typeStr(Type t);
std::string boolToString(bool v);
std::string listyToString(Value *v, char s, char e);
inline std::string listToString(Value *v) {
    return listyToString(v, '(', ')');
}
inline std::string vectorToString(Value *v) {
    return listyToString(v, '[', ']');
}

#define ValueConv(x, y)\
static inline x ## Value* to##x(Value *val) {\
    if (val->type() != y)\
        ERROR("Type mismatch: got " + typeStr(val->type()) \
                + " excpected: " + typeStr(y));\
    return static_cast<x ## Value*>(val); \
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
    virtual Value* clone() const {\
        return new x ## Value(_value);\
    }\
private:\
    t _value;\
};\
ValueConv(x, y)

ValueDef(String, Type::String, std::string, )
ValueDef(Symbol, Type::Symbol, std::string, )
ValueDef(Bool, Type::Bool, bool, boolToString)
ValueDef(Int, Type::Int, long long int, std::to_string)
ValueDef(Float, Type::Float, double, std::to_string)
ValueDef(List, Type::List, Value *, listToString)
ValueDef(Vector, Type::Vector, Value *, vectorToString)

class NilValue : public Value
{
public:
    NilValue() : Value(Type::Nil) {}
    Value *value() { return nullptr; }
    std::string toString() const { return "nil"; }
    Value *clone() const { return new NilValue(); }
};
ValueConv(Nil, Type::Nil)

class Env;
class FunctionValue : public Value
{
public:
    FunctionValue(Value *a, Value *b) :
        Value(Type::Function), bind(a), body(b) {}
    Value *params() { return bind; }
    Value *value() { return body; }
    std::string toString() const { return "#function"; }
    Value *clone() const { return new FunctionValue(bind, body); }

private:
    Value *bind;
    Value *body;
};
ValueConv(Function, Type::Function)

inline Value *iterInit(Value *o) {
    if (o->type() != Type::List && o->type() != Type::Vector)
        FATAL("Invalid iterator");
    return o->type() == Type::List
        ? toList(o)->value()
        : toVector(o)->value();
}
inline Value *iterNext(Value *v) {
    if (v == nullptr) return nullptr;
    return v->cdr();
}
