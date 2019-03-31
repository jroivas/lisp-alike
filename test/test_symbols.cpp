#include "catch.hpp"
#include "symbols.hh"
#include "value.hh"

TEST_CASE("Register and get symbol", "[symbols]") {
    Symbols s;
    auto f = [](std::string s, Value *v) {
            return new StringValue("plus");
    };
    s.registerSymbol("+", (Symbols::Handler*)&f);

    REQUIRE(s.get("+") != nullptr);
}
