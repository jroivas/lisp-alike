#pragma once

class Value
{
public:
    enum class Type {
        None,
        List
    };
    Value() : valueType(Type::None) {}
    Type type() const {
        return valueType;
    }

protected:
    Value(Type t) : valueType(t) {}

private:
    Type valueType;
};
