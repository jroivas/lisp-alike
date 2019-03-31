#include "catch.hpp"
#include "tokenize.hh"
#include "parse.hh"
#include "value.hh"
#include "eval.hh"

TEST_CASE("Eval empty", "[parse]") {
    Tokenize t("");
    Parse p(t);
    Eval e(p);

    Value *v = e.eval();
    REQUIRE(v == nullptr);
}

TEST_CASE("Eval number", "[parse]") {
    Tokenize t("42");
    Parse p(t);
    Eval e(p);

    Value *v = e.eval();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 42);
}

TEST_CASE("Eval string", "[parse]") {
    Tokenize t("\"42\"");
    Parse p(t);
    Eval e(p);

    Value *v = e.eval();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::String);
    REQUIRE(toString(v)->value() == "42");
}

TEST_CASE("Eval plus", "[parse]") {
    Tokenize t("(+ 1 2)");
    Parse p(t);
    Eval e(p);

    Value *v = e.eval();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 3);
}
