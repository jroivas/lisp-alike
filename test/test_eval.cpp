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

TEST_CASE("Eval simple if case", "[eval]") {
    Tokenize t("(if 1 2 3)");
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

TEST_CASE("Eval if nil false", "[eval]") {
    Tokenize t("(if nil (+ 2 5) (* 2 5))");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    Value *v = e.eval(p);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 10);
}

TEST_CASE("Eval do list", "[eval]") {
    Tokenize t("(do (+ 2 5) (* 2 5) 4)");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    Value *v = e.eval(p);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 4);
}

TEST_CASE("Eval do with let", "[eval]") {
    Tokenize t("(do (def! x 5) (def! y 2) (+ x y))");
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

TEST_CASE("Eval list fail", "[eval]") {
    Tokenize t("(1 2 3)");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    REQUIRE_THROWS(e.eval(p));
}

TEST_CASE("Eval simple vector", "[eval]") {
    Tokenize t("[1 2 8]");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    Value *v = e.eval(p);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Vector);

    v = toVector(v)->value();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 1);

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 2);

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 8);
}

TEST_CASE("Eval complex vector", "[eval]") {
    Tokenize t(" [1 (+ 2 4) 8]");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    Value *v = e.eval(p);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Vector);

    v = toVector(v)->value();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 1);

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 6);

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 8);
}

TEST_CASE("Eval simple fn!", "[eval]") {
    Tokenize t("(fn* [a] a)");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    Value *v = e.eval(p);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Function);
}

TEST_CASE("Eval simple fn! apply", "[eval]") {
    Tokenize t("((fn* [a] a) 7)");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    Value *v;
    CHECK_NOTHROW(v = e.eval(p));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 7);
}

TEST_CASE("Eval plus fn! apply", "[eval]") {
    Tokenize t("((fn* [a] (+ a 1)) 7)");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    Value *v;
    CHECK_NOTHROW(v = e.eval(p));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 8);
}

TEST_CASE("Eval plus fn! apply two params", "[eval]") {
    Tokenize t("((fn* [a b] (+ a b)) 2 3)");
    Parse p(t);
    Symbols s;
    Builtin b(s);
    Env n;
    Eval e(s, n);

    Value *v;
    CHECK_NOTHROW(v = e.eval(p));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 5);
}
