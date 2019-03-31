#include "catch.hpp"
#include "tokenize.hh"
#include "parse.hh"
#include "value.hh"
#include "eval.hh"
#include "builtin.hh"

TEST_CASE("Eval empty", "[eval]") {
    Tokenize t("");
    Parse p(t);
    Symbols s;
    Eval e(p, s);

    Value *v = e.eval();
    REQUIRE(v == nullptr);
}

TEST_CASE("Eval number", "[eval]") {
    Tokenize t("42");
    Parse p(t);
    Symbols s;
    Eval e(p, s);

    Value *v = e.eval();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 42);
}

TEST_CASE("Eval string", "[eval]") {
    Tokenize t("\"42\"");
    Parse p(t);
    Symbols s;
    Eval e(p, s);

    Value *v = e.eval();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::String);
    REQUIRE(toString(v)->value() == "42");
}

TEST_CASE("Eval plus", "[eval]") {
    Tokenize t("(+ 1 2)");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Eval e(p, s);

    Value *v = e.eval();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 3);
}

TEST_CASE("Eval plus many", "[eval]") {
    Tokenize t("(+ 1 2 3)");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Eval e(p, s);

    Value *v = e.eval();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 6);
}

TEST_CASE("Eval inside lists", "[eval]") {
    Tokenize t("(+ 1 (* 2 3))");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Eval e(p, s);

    Value *v = e.eval();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 7);
}

TEST_CASE("Eval inside lists first", "[eval]") {
    Tokenize t("(+ (* 2 3) 5)");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Eval e(p, s);

    Value *v = e.eval();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 11);
}
