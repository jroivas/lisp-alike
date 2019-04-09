#include "catch.hpp"
#include "tokenize.hh"
#include "parse.hh"
#include "value.hh"
#include "eval.hh"
#include "env.hh"
#include "builtin.hh"

TEST_CASE("Eval empty", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine(""));
    REQUIRE(v == nullptr);
}

TEST_CASE("Eval number", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("42"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 42);
}

TEST_CASE("Eval string", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("\"42\""));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::String);
    REQUIRE(toString(v)->value() == "42");
}

TEST_CASE("Eval plus", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("(+ 1 2)"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 3);
}

TEST_CASE("Eval plus many", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("(+ 1 2 3)"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 6);
}

TEST_CASE("Eval inside lists", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("(+ 1 (* 2 3))"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 7);
}

TEST_CASE("Eval inside lists first", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("(+ (* 2 3) 5)"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 11);
}

TEST_CASE("Eval define and val", "[eval]") {
    Symbols s;
    Builtin b(s);
    Env n;

    Value *v;
    CHECK_NOTHROW(v = evalLine(s, n, "(def! a 6)"));
    CHECK_NOTHROW(v = evalLine(s, n, "a"));

    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
}

TEST_CASE("Eval define and val bound to env", "[eval]") {
    Value *v;
    // Don't provide env so create new
    CHECK_NOTHROW(v = evalLine("(def! a 6)"));
    CHECK_NOTHROW(v = evalLine("a"));

    // Should not be avaluated since a not defined
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Symbol);
}

TEST_CASE("Eval let* simple", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("(let* (a 5) a)"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 5);
}

TEST_CASE("Eval let* complex", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine(
        "(let* ((a 5) (b 37)) (+ a b))"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 42);
}

TEST_CASE("Eval let* complex", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine(
        "(let* ((a 5) (b 7)) (* a b))"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 35);
}

TEST_CASE("Eval simple if case", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("(if 1 2 3)"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 2);
}

TEST_CASE("Eval if nil false", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("(if nil (+ 2 5) (* 2 5))"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 10);
}

TEST_CASE("Eval do list", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("(do (+ 2 5) (* 2 5) 4)"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 4);
}

TEST_CASE("Eval do with let", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine(
        "(do (def! x 5) (def! y 2) (+ x y))"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 7);
}

TEST_CASE("Eval list fail", "[eval]") {
    REQUIRE_THROWS(evalLine("(1 2 3)"));
}

TEST_CASE("Eval simple vector", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("[1 2 8]"));
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
    Value *v;
    CHECK_NOTHROW(v = evalLine("[1 (+ 2 4) 8]"));
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
    Value *v;
    CHECK_NOTHROW(v = evalLine("(fn* [a] a)"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Function);
}

TEST_CASE("Eval simple fn! apply", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("((fn* [a] a) 7)"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 7);
}

TEST_CASE("Eval plus fn! apply", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("((fn* [a] (+ a 1)) 7)"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 8);
}

TEST_CASE("Eval plus fn! apply two params", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine(
        "((fn* [a b] (+ a b)) 2 3)"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 5);
}

TEST_CASE("Eval params to list", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("(list 1 2 3)"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::List);

    v = toList(v)->value();
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 1);

    v = v->cdr();
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 2);

    v = v->cdr();
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 3);
}

TEST_CASE("Eval complex params to list", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine(
        "(list (+ 7 2) (* 2 3) (/ 10 3))"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::List);

    v = toList(v)->value();
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 9);

    v = v->cdr();
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 6);

    v = v->cdr();
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 3);
}

TEST_CASE("Eval params to empty list", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("(list)"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::List);
    REQUIRE(toList(v)->value() == nullptr);
}

TEST_CASE("Eval list param is list", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("(list? (list 1 2 3))"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Bool);
    REQUIRE(toBool(v)->value() == true);
}

TEST_CASE("Eval int not list", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("(list? 1 2 3)"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Bool);
    REQUIRE(toBool(v)->value() == false);
}

TEST_CASE("Eval list emptiness with vector", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("(empty? [1 2 3])"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Bool);
    REQUIRE(toBool(v)->value() == false);
}

TEST_CASE("Eval list emptiness", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("(empty? (list))"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Bool);
    REQUIRE(toBool(v)->value() == true);
}

TEST_CASE("Eval empty list count", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine("(count (list))"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 0);
}

TEST_CASE("Eval list count", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine(
        "(count (list 4 5 8 9 1))"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 5);
}

TEST_CASE("Eval complex list count", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine(
        "(count (list 4 (+ 5 8) 9 1))"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 4);
}

TEST_CASE("Eval list equals", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine(
        "(= (list 1 2) (list 1 2))"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Bool);
    REQUIRE(toBool(v)->value() == true);
}

TEST_CASE("Eval list not equals", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine(
        "(= (list 1 2) (list 1 3))"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Bool);
    REQUIRE(toBool(v)->value() == false);
}

TEST_CASE("Eval list equals to vector", "[eval]") {
    Value *v;
    CHECK_NOTHROW(v = evalLine(
        "(= (list 1 2 4 (+ 3 6)) [1 2 4 9])"));
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Bool);
    REQUIRE(toBool(v)->value() == true);
}
