#include "catch.hpp"
#include "tokenize.hh"
#include "parse.hh"
#include "value.hh"

TEST_CASE("Parse empty", "[parse]") {
    Tokenize t("");
    Parse p(t);
    REQUIRE(p.readForm() == nullptr);
}

TEST_CASE("Parse string", "[parse]") {
    Tokenize t("\"ab\"");
    Parse p(t);
    Value *v = p.readForm();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::String);
    REQUIRE(toString(v)->value() == "ab");
}

TEST_CASE("Parse int", "[parse]") {
    Tokenize t("42");
    Parse p(t);
    Value *v = p.readForm();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 42);
}

TEST_CASE("Parse empty list", "[parse]") {
    Tokenize t("()");
    Parse p(t);
    Value *v = p.readForm();
    REQUIRE(v != nullptr);
}
