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

    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 8);
}
