#include <iostream>
#include "catch.hpp"
#include "../src/env.h"
#include "../src/object.h"

using namespace puck;

TEST_CASE("simple env set", "[env]") {
    auto foo = object::Integer(1);
    auto bar = object::Integer(2);
    Env env;
    env.Set(foo, bar);
    REQUIRE(env.Get(foo)->EqualTo(&bar));
    REQUIRE(env.Get(bar) == nullptr);
}

TEST_CASE("outer env set", "[env]") {
    auto foo = object::Integer(1);
    auto bar = object::Integer(2);
    auto baz = object::Integer(3);
    Env outer;
    Env env(outer);
    outer.Set(foo, bar);
    REQUIRE(env.Get(foo)->EqualTo(&bar));
    REQUIRE(env.Get(bar) == nullptr);

    env.Set(foo, baz);
    REQUIRE(env.Get(foo)->EqualTo(&baz));
}
