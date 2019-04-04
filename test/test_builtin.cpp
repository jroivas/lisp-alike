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
    Env n;
    Builtin b(s);

    REQUIRE(s.get("+") != nullptr);
    Value *v1 = new IntValue(3);
    Value *v2 = new IntValue(5);
    v1->addLast(v2);
    Value *v = s.get("+")(v1, v2, &n);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 8);
}

TEST_CASE("Test plus mixed int float", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    REQUIRE(s.get("+") != nullptr);
    Value *v1 = new IntValue(3);
    Value *v2 = new FloatValue(5.14);
    v1->addLast(v2);
    Value *v = s.get("+")(v1, v2, &n);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 8.14);
}

TEST_CASE("Test plus float", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    REQUIRE(s.get("+") != nullptr);
    Value *v1 = new FloatValue(1.2);
    Value *v2 = new FloatValue(4.7);
    v1->addLast(v2);
    Value *v = s.get("+")(v1, v2, &n);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 5.9);
}

TEST_CASE("Test plus faulty", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    REQUIRE(s.get("+") != nullptr);
    Value *v1 = new IntValue(5);
    Value *v2 = new StringValue("tmp");
    v1->addLast(v2);
    REQUIRE_THROWS(s.get("+")(v1, v2, &n));
}

TEST_CASE("Test minus", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    REQUIRE(s.get("-") != nullptr);
    Value *v1 = new IntValue(3);
    Value *v2 = new IntValue(5);
    v1->addLast(v2);
    Value *v = s.get("-")(v1, v2, &n);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == -2);
}

TEST_CASE("Test minus mixed int float", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    REQUIRE(s.get("-") != nullptr);
    Value *v1 = new IntValue(10);
    Value *v2 = new FloatValue(5.14);
    v1->addLast(v2);
    Value *v = s.get("-")(v1, v2, &n);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 4.86);
}

TEST_CASE("Test minus float", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    REQUIRE(s.get("-") != nullptr);
    Value *v1 = new FloatValue(4.2);
    Value *v2 = new FloatValue(1.7);
    v1->addLast(v2);
    Value *v = s.get("-")(v1, v2, &n);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 2.5);
}

TEST_CASE("Test mul", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    REQUIRE(s.get("*") != nullptr);
    Value *v1 = new IntValue(3);
    Value *v2 = new IntValue(5);
    v1->addLast(v2);
    Value *v = s.get("*")(v1, v2, &n);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 15);
}

TEST_CASE("Test mul mixed int float", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    REQUIRE(s.get("*") != nullptr);
    Value *v1 = new IntValue(10);
    Value *v2 = new FloatValue(5.14);
    v1->addLast(v2);
    Value *v = s.get("*")(v1, v2, &n);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 51.4);
}

TEST_CASE("Test mul float", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    REQUIRE(s.get("*") != nullptr);
    Value *v1 = new FloatValue(4.2);
    Value *v2 = new FloatValue(1.7);
    v1->addLast(v2);
    Value *v = s.get("*")(v1, v2, &n);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 7.14);
}

TEST_CASE("Test div", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    REQUIRE(s.get("/") != nullptr);
    Value *v1 = new IntValue(20);
    Value *v2 = new IntValue(3);
    v1->addLast(v2);
    Value *v = s.get("/")(v1, v2, &n);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 6);
}

TEST_CASE("Test div mixed int float", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    REQUIRE(s.get("/") != nullptr);
    Value *v1 = new IntValue(55);
    Value *v2 = new FloatValue(5.5);
    v1->addLast(v2);
    Value *v = s.get("/")(v1, v2, &n);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 10);
}

TEST_CASE("Test div float", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    REQUIRE(s.get("/") != nullptr);
    Value *v1 = new FloatValue(23.1);
    Value *v2 = new FloatValue(4.2);
    v1->addLast(v2);
    Value *v = s.get("/")(v1, v2, &n);

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 5.5);
}

TEST_CASE("Test div by zero float", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    REQUIRE(s.get("/") != nullptr);
    Value *v1 = new FloatValue(10);
    Value *v2 = new IntValue(0);
    v1->addLast(v2);
    REQUIRE_THROWS(s.get("/")(v1, v2, &n));
}

TEST_CASE("Test div by zero", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    REQUIRE(s.get("/") != nullptr);
    Value *v1 = new IntValue(10);
    Value *v2 = new IntValue(0);
    v1->addLast(v2);
    REQUIRE_THROWS(s.get("/")(v1, v2, &n));
}

TEST_CASE("Test def!", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    REQUIRE(s.get("def!")!= nullptr);
    Value *v1 = new SymbolValue("a");
    Value *v2 = new IntValue(6);
    REQUIRE(s.get("def!")(v1, v2, &n) == v2);

    REQUIRE(n.get("a") != nullptr);
    REQUIRE(n.get("a") == v2);
}

TEST_CASE("Test simple let*", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    REQUIRE(s.get("let*")!= nullptr);
    Value *v1_1 = new SymbolValue("a");
    Value *v1_2 = new IntValue(2);
    v1_1->addLast(v1_2);
    Value *v1 = new ListValue(v1_1);

    Value *v2 = new SymbolValue("a");
    Value *res = s.get("let*")(v1, v2, &n);
    REQUIRE(res != nullptr);
    REQUIRE(res->type() == Type::Int);
    REQUIRE(toInt(res)->value() == 2);
}

TEST_CASE("Test list let*", "[builtin]") {
    Symbols s;
    Env n;
    Builtin b(s);

    /*
     (let*
        ((a 2)
         (b 7))
         (+ a b)
     )
     */
    REQUIRE(s.get("let*")!= nullptr);
    Value *v1_1 = new SymbolValue("a");
    Value *v1_2 = new IntValue(2);
    v1_1->addLast(v1_2);

    Value *v2_1 = new SymbolValue("b");
    Value *v2_2 = new IntValue(7);
    v2_1->addLast(v2_2);

    Value *v1 = new ListValue(v1_1);
    Value *v2 = new ListValue(v2_1);
    v1->addLast(v2);
    Value *v3 = new ListValue(v1);

    Value *v4_1 = new SymbolValue("+");
    Value *v4_2 = new SymbolValue("a");
    Value *v4_3 = new SymbolValue("b");
    v4_1->addLast(v4_2);
    v4_1->addLast(v4_3);
    Value *v4 = new ListValue(v4_1);

    Value *res = s.get("let*")(v3, v4, &n);
    REQUIRE(res != nullptr);
    REQUIRE(res->type() == Type::Int);
    REQUIRE(toInt(res)->value() == 9);
}
