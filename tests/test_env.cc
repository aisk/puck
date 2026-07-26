#include <stdlib.h>
#include <string.h>
#include <catch2/catch.hpp>
#include "../src/env.h"

static rv_obj* symbol(const char* value) {
    return rv_symbol_new(strdup(value));
}

TEST_CASE("simple env set", "[env]") {
    rv_env* env = rv_env_new(NULL);
    rv_obj* value = rv_integer_new(2);
    rv_env_set(env, symbol("foo"), value);
    REQUIRE(rv_env_get(env, symbol("foo")) == value);
    REQUIRE(rv_env_get(env, symbol("bar")) == NULL);
}

TEST_CASE("outer env set", "[env]") {
    rv_env* outer = rv_env_new(NULL);
    rv_env* env = rv_env_new(outer);
    rv_obj* outer_value = rv_integer_new(2);
    rv_obj* inner_value = rv_integer_new(3);

    rv_env_set(outer, symbol("foo"), outer_value);
    REQUIRE(rv_env_get(env, symbol("foo")) == outer_value);

    rv_env_set(env, symbol("foo"), inner_value);
    REQUIRE(rv_env_get(env, symbol("foo")) == inner_value);
}
