#include "catch.hpp"
#include "tokenize.hh"
#include "parse.hh"
#include "value.hh"
#include "eval.hh"
#include "env.hh"
#include "builtin.hh"

TEST_CASE("Eval empty", "[eval]") {
    Tokenize t("");
    Parse p(t);
    Symbols s;
    Env n;
    Eval e(s, n);

    Value *v = e.eval(p);
    REQUIRE(v == nullptr);
}

TEST_CASE("Eval number", "[eval]") {
    Tokenize t("42");
    Parse p(t);
    Symbols s;
    Env n;
    Eval e(s, n);

    Value *v = e.eval(p);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 42);
}

TEST_CASE("Eval string", "[eval]") {
    Tokenize t("\"42\"");
    Parse p(t);
    Symbols s;
    Env n;
    Eval e(s, n);

    Value *v = e.eval(p);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::String);
    REQUIRE(toString(v)->value() == "42");
}

TEST_CASE("Eval plus", "[eval]") {
    Tokenize t("(+ 1 2)");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    Value *v = e.eval(p);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 3);
}

TEST_CASE("Eval plus many", "[eval]") {
    Tokenize t("(+ 1 2 3)");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    Value *v = e.eval(p);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 6);
}

TEST_CASE("Eval inside lists", "[eval]") {
    Tokenize t("(+ 1 (* 2 3))");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    Value *v = e.eval(p);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 7);
}

TEST_CASE("Eval inside lists first", "[eval]") {
    Tokenize t("(+ (* 2 3) 5)");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    Value *v = e.eval(p);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 11);
}

TEST_CASE("Eval define and val", "[eval]") {
    Tokenize t1("(def! a 6)");
    Parse p1(t1);
    Tokenize t2("a");
    Parse p2(t2);

    Symbols s;
    Builtin b(s);
    Env n;

    Eval e1(s, n);
    e1.eval(p1);

    Eval e2(s, n);
    Value *v = e2.eval(p2);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
}

TEST_CASE("Eval let* simple", "[eval]") {
    Tokenize t("(let* (a 5) a)");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    Value *v = e.eval(p);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 5);
}

TEST_CASE("Eval let* complex", "[eval]") {
    Tokenize t("(let* ((a 5) (b 37)) (+ a b))");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    Value *v = e.eval(p);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 42);
}

TEST_CASE("Eval let* complex", "[eval]") {
    Tokenize t("(let* ((a 5) (b 7)) (* a b))");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    Value *v = e.eval(p);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 35);
}

TEST_CASE("Eval if case", "[eval]") {
    Tokenize t("(if (1) (2) (3))");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    Value *v = e.eval(p);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 2);
}
