#include "catch.hpp"
#include "symbols.hh"
#include "value.hh"

TEST_CASE("Get nonexist symbol", "[symbols]") {
    Symbols s;
    REQUIRE(s.get("+") == nullptr);
}

TEST_CASE("Register and get symbol", "[symbols]") {
    Symbols s;
    auto f = [](std::string s, Value *v) -> Value* {
        return new StringValue("got plus");
    };
    s.registerSymbol("+", f);

    REQUIRE(s.get("+") != nullptr);
    Value *v = s.get("+")("", nullptr);
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::String);
    REQUIRE(toString(v)->value() == "got plus");
}
