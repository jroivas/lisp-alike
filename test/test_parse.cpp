#include "catch.hpp"
#include "tokenize.hh"
#include "parse.hh"
#include "value.hh"

TEST_CASE("Parse empty", "[parse]") {
    Tokenize t("");
    Parse p(t);
    REQUIRE(p.readForm() == nullptr);
}

TEST_CASE("Parse empty list", "[parse]") {
    Tokenize t("()");
    Parse p(t);
    Value *v = p.readForm();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Value::Type::List);
}
