#include <string.h>
#include <catch2/catch.hpp>
#include "../src/object.h"

TEST_CASE("integer", "[object]") {
    rv_obj* obj = rv_integer_new(1);
    REQUIRE(obj->t == RV_TYPE_INTEGER);
    REQUIRE(obj->integer_v == 1);
    rv_obj_free(obj);
}

TEST_CASE("real", "[object]") {
    rv_obj* obj = rv_real_new(0.618);
    REQUIRE(obj->t == RV_TYPE_REAL);
    REQUIRE(obj->real_v == Approx(0.618));
    rv_obj_free(obj);
}

TEST_CASE("bool", "[object]") {
    rv_obj* obj = rv_bool_new(true);
    REQUIRE(obj->t == RV_TYPE_BOOL);
    REQUIRE(obj->bool_v);
    rv_obj_free(obj);
}

TEST_CASE("pair", "[object]") {
    rv_obj* nil = rv_pair_new(NULL, NULL);
    REQUIRE(rv_obj_is_nil(nil));
    rv_obj_free(nil);
}

TEST_CASE("symbol", "[object]") {
    char* value = strdup("foo");
    rv_obj* obj = rv_symbol_new(value);
    REQUIRE(obj->t == RV_TYPE_SYMBOL);
    REQUIRE(strcmp(obj->string_v, "foo") == 0);
    rv_obj_free(obj);
}

TEST_CASE("string", "[object]") {
    char* value = strdup("hello world!");
    rv_obj* obj = rv_string_new(value);
    REQUIRE(obj->t == RV_TYPE_STRING);
    REQUIRE(strcmp(obj->string_v, "hello world!") == 0);
    free(obj->string_v);
    free(obj);
}
