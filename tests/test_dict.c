#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "../src/object.h"
#include "../src/dict.h"

extern void expect(rv_obj* obj, rv_type type, void* value);
extern void expect_nil(rv_obj* obj);

int test_dict_new() {
    rv_dict_new();

    return 0;
}

int test_dict_set() {
    rv_dict* dict = rv_dict_new();
    rv_dict_set(dict, rv_symbol_new("true"), rv_bool_new(true));
    rv_dict_set(dict, rv_symbol_new("false"), rv_bool_new(false));
    return 0;
}

int test_dict_get() {
    rv_dict* dict = rv_dict_new();
    rv_obj* obj;

    rv_obj* symbols[5] = {
        rv_symbol_new("zero"),
        rv_symbol_new("one"), 
        rv_symbol_new("two"), 
        rv_symbol_new("tree"),
        rv_symbol_new("four"),
    };
    for (int i = 0; i < 5; i++) {
        rv_dict_set(dict, symbols[i], rv_integer_new(i));
    }
    for (int i = 0; i < 5; i++) {
        obj = rv_dict_get(dict, symbols[i]);
        expect(obj, RV_TYPE_INTEGER, &i);
    }

    obj = rv_dict_get(dict, rv_symbol_new("nothing"));
    assert(obj == NULL);

    return 0;
}

int test_dict_update() {
    rv_dict* dict = rv_dict_new();
    rv_obj* obj;

    rv_dict_set(dict, rv_symbol_new("foo"), rv_symbol_new("bar"));
    obj = rv_dict_get(dict, rv_symbol_new("foo"));
    expect(obj, RV_TYPE_SYMBOL, "bar");

    rv_dict_set(dict, rv_symbol_new("foo"), rv_symbol_new("baz"));
    obj = rv_dict_get(dict, rv_symbol_new("foo"));
    expect(obj, RV_TYPE_SYMBOL, "baz");

    return 0;
}

int test_dict() {
    rv_dict* dict = rv_dict_new();
    rv_dict_set(dict, rv_symbol_new("true"), rv_bool_new(true));
    rv_dict_set(dict, rv_symbol_new("false"), rv_bool_new(false));
    rv_dict_set(dict, rv_symbol_new("one"), rv_integer_new(1));
    rv_dict_set(dict, rv_symbol_new("two"), rv_integer_new(2));
    rv_dict_set(dict, rv_symbol_new("tree"), rv_integer_new(3));
    rv_dict_set(dict, rv_symbol_new("four"), rv_integer_new(4));
    rv_dict_set(dict, rv_symbol_new("five"), rv_integer_new(5));
    rv_dict_set(dict, rv_symbol_new("foo"), rv_symbol_new("bar"));

    rv_obj* obj;
    obj = rv_dict_get(dict, rv_symbol_new("true"));
    bool vtrue = true;
    expect(obj, RV_TYPE_BOOL, &vtrue);
    
    obj = rv_dict_get(dict, rv_symbol_new("nothing"));
    assert(obj == NULL);

    obj = rv_dict_get(dict, rv_symbol_new("foo"));
    expect(obj, RV_TYPE_SYMBOL, "bar");
    rv_dict_set(dict, rv_symbol_new("foo"), rv_symbol_new("baz"));


    obj = rv_dict_get(dict, rv_symbol_new("foo"));
    expect(obj, RV_TYPE_SYMBOL, "baz");

    return 0;
}
