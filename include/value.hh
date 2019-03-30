#pragma once

enum class Type {
    None,
    String,
    Int
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
x ## Value* to##x(Value *val) { return (x ## Value*)val; }

ValueDef(String, Type::String, std::string)
ValueDef(Int, Type::Int, long long int)
