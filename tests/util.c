#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../src/object.h"

void expect(rv_obj* obj, rv_type type, void* value) {
    assert(obj);
    assert(obj->t == type);
    switch (obj->t) {
    case RV_TYPE_INTEGER:
        assert(obj->integer_v == *(int*)value);
        break;
    case RV_TYPE_REAL:
        assert(obj->real_v == *(double*)value);
        break;
    case RV_TYPE_BOOL:
        assert(obj->bool_v == *(bool*)value);
        break;
    case RV_TYPE_SYMBOL:
    case RV_TYPE_STRING:
        assert(strcmp(obj->string_v, (char*)value) == 0);
        break;
    default:
        // TODO
        puts("not implemented");
        exit(1);
    }
}

void expect_nil(rv_obj* obj) {
    assert(obj);
    assert(obj->t == RV_TYPE_PAIR);
    assert(obj->pair_v.car == NULL);
    assert(obj->pair_v.cdr == NULL);
}
