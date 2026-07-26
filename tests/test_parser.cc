#include <stdlib.h>
#include <string.h>
#include <catch2/catch.hpp>
#include "../src/parser.h"

static rv_obj* parse(const char* source) {
    rv_parser* parser = rv_parser_new(source);
    rv_obj* result = rv_parse(parser);
    rv_parser_free(parser);
    return result;
}

TEST_CASE("parse integer", "[parser]") {
    rv_obj* obj = parse("  -42 ");
    REQUIRE(obj->t == RV_TYPE_INTEGER);
    REQUIRE(obj->integer_v == -42);
}

TEST_CASE("parse real", "[parser]") {
    rv_obj* obj = parse(" -3.14 ");
    REQUIRE(obj->t == RV_TYPE_REAL);
    REQUIRE(obj->real_v == Approx(-3.14));
}

TEST_CASE("parse bool", "[parser]") {
    rv_obj* obj = parse(" #t ");
    REQUIRE(obj->t == RV_TYPE_BOOL);
    REQUIRE(obj->bool_v);
}

TEST_CASE("parse symbol", "[parser]") {
    rv_obj* obj = parse("foo ");
    REQUIRE(obj->t == RV_TYPE_SYMBOL);
    REQUIRE(strcmp(obj->string_v, "foo") == 0);
}

TEST_CASE("parse empty list", "[parser]") {
    rv_obj* obj = parse(" ( ) ");
    REQUIRE(rv_obj_is_nil(obj));
}

TEST_CASE("parse simple list", "[parser]") {
    rv_obj* obj = parse("(1 2 3)");
    REQUIRE(obj->pair_v.car->integer_v == 1);
    REQUIRE(obj->pair_v.cdr->pair_v.car->integer_v == 2);
    REQUIRE(obj->pair_v.cdr->pair_v.cdr->pair_v.car->integer_v == 3);
}

TEST_CASE("parse nested list", "[parser]") {
    rv_obj* obj = parse("(1 (2 3))");
    rv_obj* inner = obj->pair_v.cdr->pair_v.car;
    REQUIRE(inner->pair_v.car->integer_v == 2);
    REQUIRE(inner->pair_v.cdr->pair_v.car->integer_v == 3);
}

TEST_CASE("parse string", "[parser]") {
    rv_obj* obj = parse("\"hello world!\"");
    REQUIRE(obj->t == RV_TYPE_STRING);
    REQUIRE(strcmp(obj->string_v, "hello world!") == 0);
}

TEST_CASE("parse string escapes", "[parser]") {
    rv_obj* obj = parse("\"hello\\nworld\\t\\\"ok\\\"\"");
    REQUIRE(strcmp(obj->string_v, "hello\nworld\t\"ok\"") == 0);
}

TEST_CASE("parser reports malformed input", "[parser]") {
    rv_parser* parser = rv_parser_new("(1 2\n 3");
    REQUIRE(rv_parse(parser) == NULL);
    REQUIRE(strcmp(rv_parser_error(parser), "expected ')' to close list") == 0);
    REQUIRE(rv_parser_error_line(parser) == 2);
    REQUIRE(rv_parser_error_column(parser) == 3);
    rv_parser_free(parser);

    parser = rv_parser_new(")");
    REQUIRE(rv_parse(parser) == NULL);
    REQUIRE(strcmp(rv_parser_error(parser), "unexpected ')'") == 0);
    rv_parser_free(parser);

    parser = rv_parser_new("#x");
    REQUIRE(rv_parse(parser) == NULL);
    REQUIRE(strcmp(rv_parser_error(parser), "expected '#t' or '#f'") == 0);
    rv_parser_free(parser);
}
