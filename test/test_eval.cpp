#include "catch.hpp"
#include "tokenize.hh"
#include "parse.hh"
#include "value.hh"
#include "eval.hh"

TEST_CASE("Eval empty", "[parse]") {
    Tokenize t("");
    Parse p(t);
    Eval e(p);

    REQUIRE(e.eval() != nullptr);
}

