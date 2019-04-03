#include "catch.hpp"
#include "env.hh"

TEST_CASE("Env basic set get", "[env]") {
    Env e;
    e.set("test", new StringValue("testValue"));

    Value *v = e.get("test");
    REQUIRE( v != nullptr );
    REQUIRE( v->type() == Type::String );
    REQUIRE( toString(v)->value() == "testValue" );
}
