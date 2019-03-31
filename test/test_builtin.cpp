#include "catch.hpp"
#include "builtin.hh"
#include "symbols.hh"
#include "value.hh"

TEST_CASE("Test builtin registers symbols", "[builtin]") {
    Symbols s;
    Builtin b(s);

    REQUIRE(s.get("+") != nullptr);
}

TEST_CASE("Test plus", "[builtin]") {
    Symbols s;
    Builtin b(s);

    REQUIRE(s.get("+") != nullptr);
    Value *v1 = new IntValue(3);
    Value *v2 = new IntValue(5);
    v1->addLast(v2);
    Value *v = s.get("+")(v1);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 8);
}

TEST_CASE("Test plus mixed int float", "[builtin]") {
    Symbols s;
    Builtin b(s);

    REQUIRE(s.get("+") != nullptr);
    Value *v1 = new IntValue(3);
    Value *v2 = new FloatValue(5.14);
    v1->addLast(v2);
    Value *v = s.get("+")(v1);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 8.14);
}

TEST_CASE("Test plus float", "[builtin]") {
    Symbols s;
    Builtin b(s);

    REQUIRE(s.get("+") != nullptr);
    Value *v1 = new FloatValue(1.2);
    Value *v2 = new FloatValue(4.7);
    v1->addLast(v2);
    Value *v = s.get("+")(v1);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 5.9);
}

TEST_CASE("Test plus faulty", "[builtin]") {
    Symbols s;
    Builtin b(s);

    REQUIRE(s.get("+") != nullptr);
    Value *v1 = new IntValue(5);
    Value *v2 = new StringValue("tmp");
    v1->addLast(v2);
    REQUIRE_THROWS(s.get("+")(v1));
}

TEST_CASE("Test minus", "[builtin]") {
    Symbols s;
    Builtin b(s);

    REQUIRE(s.get("-") != nullptr);
    Value *v1 = new IntValue(3);
    Value *v2 = new IntValue(5);
    v1->addLast(v2);
    Value *v = s.get("-")(v1);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == -2);
}

TEST_CASE("Test minus mixed int float", "[builtin]") {
    Symbols s;
    Builtin b(s);

    REQUIRE(s.get("-") != nullptr);
    Value *v1 = new IntValue(10);
    Value *v2 = new FloatValue(5.14);
    v1->addLast(v2);
    Value *v = s.get("-")(v1);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 4.86);
}

TEST_CASE("Test minus float", "[builtin]") {
    Symbols s;
    Builtin b(s);

    REQUIRE(s.get("-") != nullptr);
    Value *v1 = new FloatValue(4.2);
    Value *v2 = new FloatValue(1.7);
    v1->addLast(v2);
    Value *v = s.get("-")(v1);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 2.5);
}

TEST_CASE("Test mul", "[builtin]") {
    Symbols s;
    Builtin b(s);

    REQUIRE(s.get("*") != nullptr);
    Value *v1 = new IntValue(3);
    Value *v2 = new IntValue(5);
    v1->addLast(v2);
    Value *v = s.get("*")(v1);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 15);
}

TEST_CASE("Test mul mixed int float", "[builtin]") {
    Symbols s;
    Builtin b(s);

    REQUIRE(s.get("*") != nullptr);
    Value *v1 = new IntValue(10);
    Value *v2 = new FloatValue(5.14);
    v1->addLast(v2);
    Value *v = s.get("*")(v1);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 51.4);
}

TEST_CASE("Test mul float", "[builtin]") {
    Symbols s;
    Builtin b(s);

    REQUIRE(s.get("*") != nullptr);
    Value *v1 = new FloatValue(4.2);
    Value *v2 = new FloatValue(1.7);
    v1->addLast(v2);
    Value *v = s.get("*")(v1);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 7.14);
}

TEST_CASE("Test div", "[builtin]") {
    Symbols s;
    Builtin b(s);

    REQUIRE(s.get("/") != nullptr);
    Value *v1 = new IntValue(20);
    Value *v2 = new IntValue(3);
    v1->addLast(v2);
    Value *v = s.get("/")(v1);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 6);
}

TEST_CASE("Test div mixed int float", "[builtin]") {
    Symbols s;
    Builtin b(s);

    REQUIRE(s.get("/") != nullptr);
    Value *v1 = new IntValue(55);
    Value *v2 = new FloatValue(5.5);
    v1->addLast(v2);
    Value *v = s.get("/")(v1);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 10);
}

TEST_CASE("Test div float", "[builtin]") {
    Symbols s;
    Builtin b(s);

    REQUIRE(s.get("/") != nullptr);
    Value *v1 = new FloatValue(23.1);
    Value *v2 = new FloatValue(4.2);
    v1->addLast(v2);
    Value *v = s.get("/")(v1);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 5.5);
}