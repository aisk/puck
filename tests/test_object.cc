#include <string.h>
#include <catch2/catch.hpp>
#include "../src/object.h"
#include "../src/state.h"

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

TEST_CASE("object equality", "[object]") {
    rv_obj* one = rv_integer_new(1);
    rv_obj* another_one = rv_integer_new(1);
    rv_obj* two = rv_integer_new(2);
    rv_obj* nil = rv_pair_new(NULL, NULL);
    rv_obj* pair = rv_pair_new(one, nil);
    rv_obj* same_pair = rv_pair_new(another_one, rv_pair_new(NULL, NULL));

    REQUIRE(rv_obj_equal(one, another_one));
    REQUIRE_FALSE(rv_obj_equal(one, two));
    REQUIRE(rv_obj_equal(pair, same_pair));
}

TEST_CASE("object to string", "[object]") {
    rv_obj* pair = rv_pair_new(rv_integer_new(1), rv_pair_new(NULL, NULL));
    char* value = rv_obj_to_string(pair);
    REQUIRE(strcmp(value, "(1 . ( . ))") == 0);
    free(value);
}

TEST_CASE("symbol", "[object]") {
    char value[] = "foo";
    rv_obj* obj = rv_symbol_new(value);
    value[0] = 'x';
    REQUIRE(obj->t == RV_TYPE_SYMBOL);
    REQUIRE(strcmp(obj->string_v, "foo") == 0);
    rv_obj_free(obj);
}

TEST_CASE("string", "[object]") {
    char value[] = "hello world!";
    rv_obj* obj = rv_string_new(value);
    REQUIRE(obj->t == RV_TYPE_STRING);
    REQUIRE(strcmp(obj->string_v, "hello world!") == 0);
    rv_obj_free(obj);
}

TEST_CASE("state owns allocated objects", "[state]") {
    rv_state* previous = rv_state_current();
    rv_state* state = rv_state_new();
    rv_state_set_current(state);
    rv_integer_new(1);
    rv_string_new("value");
    REQUIRE(rv_state_object_count(state) == 2);
    rv_state_free(state);
    rv_state_set_current(previous);
}
