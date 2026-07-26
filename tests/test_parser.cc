#include <stdlib.h>
#include <string.h>
#include <catch2/catch.hpp>
#include "../src/parser.h"

static rv_obj* parse(char* source) {
    rv_parser* parser = rv_parser_new(source);
    rv_obj* result = rv_parse(parser);
    free(parser);
    return result;
}

TEST_CASE("parse integer", "[parser]") {
    rv_obj* obj = parse(const_cast<char *>("  -42 "));
    REQUIRE(obj->t == RV_TYPE_INTEGER);
    REQUIRE(obj->integer_v == -42);
}

TEST_CASE("parse real", "[parser]") {
    rv_obj* obj = parse(const_cast<char *>(" -3.14 "));
    REQUIRE(obj->t == RV_TYPE_REAL);
    REQUIRE(obj->real_v == Approx(-3.14));
}

TEST_CASE("parse bool", "[parser]") {
    rv_obj* obj = parse(const_cast<char *>(" #t "));
    REQUIRE(obj->t == RV_TYPE_BOOL);
    REQUIRE(obj->bool_v);
}

TEST_CASE("parse symbol", "[parser]") {
    rv_obj* obj = parse(const_cast<char *>("foo "));
    REQUIRE(obj->t == RV_TYPE_SYMBOL);
    REQUIRE(strcmp(obj->string_v, "foo") == 0);
}

TEST_CASE("parse empty list", "[parser]") {
    rv_obj* obj = parse(const_cast<char *>(" ( ) "));
    REQUIRE(rv_obj_is_nil(obj));
}

TEST_CASE("parse simple list", "[parser]") {
    rv_obj* obj = parse(const_cast<char *>("(1 2 3)"));
    REQUIRE(obj->pair_v.car->integer_v == 1);
    REQUIRE(obj->pair_v.cdr->pair_v.car->integer_v == 2);
    REQUIRE(obj->pair_v.cdr->pair_v.cdr->pair_v.car->integer_v == 3);
}

TEST_CASE("parse nested list", "[parser]") {
    rv_obj* obj = parse(const_cast<char *>("(1 (2 3))"));
    rv_obj* inner = obj->pair_v.cdr->pair_v.car;
    REQUIRE(inner->pair_v.car->integer_v == 2);
    REQUIRE(inner->pair_v.cdr->pair_v.car->integer_v == 3);
}

TEST_CASE("parse string", "[parser]") {
    rv_obj* obj = parse(const_cast<char *>("\"hello world!\""));
    REQUIRE(obj->t == RV_TYPE_STRING);
    REQUIRE(strcmp(obj->string_v, "hello world!") == 0);
}
