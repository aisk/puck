#include <stdio.h>
#include <string.h>
#include <memory>
#include "../src/object.h"
#include "catch.hpp"

using namespace rv;

TEST_CASE("integer", "[object]") {
    auto one = rv::object::Integer(1);
    auto two = rv::object::Integer(2);
    auto anotherOne = object::Integer(1);

    REQUIRE(one.GetType() == rv::object::type::INTEGER);
    REQUIRE(one.EqualTo(&two) == false);
    REQUIRE(one.EqualTo(&anotherOne) == true);
}

TEST_CASE("real", "[object]") {
    auto obj = object::Real(0.618);
    REQUIRE(obj.GetType() == object::type::REAL);
}

TEST_CASE("bool", "[object]") {
    auto True = object::Bool(true);
    REQUIRE(True.GetType() == object::type::BOOL);
    REQUIRE(strcmp(True.ToString(), "#t") == 0);
}

TEST_CASE("pair", "[object]") {
    auto nil = object::Pair(nullptr, nullptr);
    auto one = object::Integer(1);
    auto two = object::Integer(2);
    auto three = object::Integer(3);

    REQUIRE(nil.GetType() == object::type::PAIR);
    REQUIRE(strcmp(nil.ToString(), "( . )") == 0);
    REQUIRE(nil.EqualTo(new object::Pair(nullptr, nullptr)));

    auto pair = object::Pair(&one, &nil);
    auto anotherPair = object::Pair(&one, &nil);
    REQUIRE(pair.EqualTo(&anotherPair));

    pair = object::Pair(&one, new object::Pair(&two, new object::Pair(&three, &nil)));
    REQUIRE(strcmp(pair.ToString(), "(1 . (2 . (3 . ( . ))))") == 0);
    REQUIRE(pair.EqualTo(new object::Pair(&one, new object::Pair(&two, new object::Pair(&three, &nil)))));
}

TEST_CASE("symbol", "[object]") {
    char *str = const_cast<char *>("foo");
    auto obj = object::Symbol(str);
    REQUIRE(obj.GetType() == object::type::SYMBOL);
    REQUIRE(strcmp(obj.ToString(), "'foo") == 0);
}

TEST_CASE("string", "[object]") {
    char *str = const_cast<char *>("hello world!");
    auto obj = object::String(str);
    REQUIRE(obj.GetType() == object::type::STRING);
    REQUIRE(strcmp(obj.ToString(), "\"hello world!\"") == 0);
}
