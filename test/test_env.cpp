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

TEST_CASE("Nested envs", "[env]") {
    Env e1;
    Env e2(&e1);

    e1.set("test", new StringValue("testValue"));
    e1.set("test2", new IntValue(42));

    Value *v = e2.get("test2");
    REQUIRE( v != nullptr );
    REQUIRE( v->type() == Type::Int );
    REQUIRE( toInt(v)->value() == 42);

    e2.set("test2", new StringValue("testValue2"));
    v = e2.get("test2");
    REQUIRE( v != nullptr );
    REQUIRE( v->type() == Type::String );
    REQUIRE( toString(v)->value() == "testValue2" );

    v = e1.get("test2");
    REQUIRE( v != nullptr );
    REQUIRE( v->type() == Type::Int );
    REQUIRE( toInt(v)->value() == 42);

    v = e2.get("test");
    REQUIRE( v != nullptr );
    REQUIRE( v->type() == Type::String );
    REQUIRE( toString(v)->value() == "testValue" );
}

TEST_CASE("Env binding", "[env]") {
    Value *b = new SymbolValue("b");
    Value *d = new SymbolValue("d");
    b->addLast(d);

    Value *p1 = new IntValue(8);
    Value *p2 = new IntValue(42);
    Value *p3 = new IntValue(8);
    p1->addLast(p2);
    p1->addLast(p3);

    Env *f;
    REQUIRE_NOTHROW(f = Env::bind(b, p1));
    REQUIRE(f != nullptr);
    REQUIRE(f->get("a") == nullptr);
    REQUIRE(f->get("b") != nullptr);
    REQUIRE(f->get("b")->type() == Type::Int);
    REQUIRE(toInt(f->get("b"))->value() == 8);
    REQUIRE(f->get("c") == nullptr);
    REQUIRE(f->get("d") != nullptr);
    REQUIRE(f->get("d")->type() == Type::Int);
    REQUIRE(toInt(f->get("d"))->value() == 42);
}

TEST_CASE("Env binding list", "[env]") {
    Value *a = new SymbolValue("b");
    Value *b = new SymbolValue("&");
    Value *c = new SymbolValue("c");
    a->addLast(b);
    a->addLast(c);

    Value *p1 = new IntValue(99);
    Value *p2 = new IntValue(8);
    Value *p3 = new IntValue(42);
    Value *p4 = new IntValue(123);
    p1->addLast(p2);
    p1->addLast(p3);
    p1->addLast(p4);

    Env *f;
    REQUIRE_NOTHROW(f = Env::bind(a, p1));
    REQUIRE(f != nullptr);
    REQUIRE(f->get("a") == nullptr);
    REQUIRE(f->get("b") != nullptr);
    REQUIRE(f->get("b")->type() == Type::Int);
    REQUIRE(toInt(f->get("b"))->value() == 99);

    Value *v = f->get("c");
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::List);

    v = toList(v)->value();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 8);

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 42);

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 123);
}

TEST_CASE("Env binding list too many binds", "[env]") {
    Value *a = new SymbolValue("b");
    Value *b = new SymbolValue("&");
    Value *c = new SymbolValue("c");
    Value *d = new SymbolValue("d");
    a->addLast(b);
    a->addLast(c);
    a->addLast(d);

    Value *p1 = new IntValue(99);
    Value *p2 = new IntValue(8);
    p1->addLast(p2);

    REQUIRE_THROWS(Env::bind(a, p1));
}

TEST_CASE("Env binding list too little binds", "[env]") {
    Value *a = new SymbolValue("b");
    Value *b = new SymbolValue("&");
    a->addLast(b);

    Value *p1 = new IntValue(99);
    Value *p2 = new IntValue(8);
    p1->addLast(p2);

    REQUIRE_THROWS(Env::bind(a, p1));
}
