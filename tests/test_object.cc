#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "../src/object.h"

extern void expect(rv_obj* obj, rv_type type, void* value);
extern void expect_nil(rv_obj* obj);

int test_obj_is_nil() {
    assert(true == rv_obj_is_nil(rv_pair_new(NULL, NULL)));

    assert(false == rv_obj_is_nil(rv_string_new("imnotpair")));

    assert(false == rv_obj_is_nil(rv_pair_new(NULL, rv_integer_new(1))));

    assert(false == rv_obj_is_nil(rv_pair_new(rv_integer_new(1), NULL)));

    return 0;
}
