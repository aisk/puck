#include <assert.h>

#include "../src/object.h"
#include "../src/env.h"

extern void expect(rv_obj* obj, rv_type type, void* value);
extern void expect_nil(rv_obj* obj);

int test_env_new() {
    rv_env* env = rv_env_new(NULL);
    rv_env_new(env);

    return 0;
}

int test_env_get() {
    rv_obj* obj;
    rv_env* env = rv_env_new(NULL);
    rv_env* outer_env = rv_env_new(env);

    rv_obj* foo = rv_symbol_new("foo");
    rv_obj* bar = rv_symbol_new("bar");
    rv_obj* baz = rv_symbol_new("baz");

    rv_env_set(env, foo, bar);
    obj = rv_env_get(env, foo);
    assert(obj == bar);

    obj = rv_env_get(env, bar);
    assert(obj == NULL);

    obj = rv_env_get(outer_env, foo);
    assert(obj == bar);
    
    rv_env_set(outer_env, foo, baz);
    obj = rv_env_get(outer_env, foo);
    assert(obj == baz);

    return 0;
}
