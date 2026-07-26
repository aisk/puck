#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdarg.h>
#include "object.h"
#include "state.h"

static char* rv_strdup(const char* value) {
    size_t length = strlen(value) + 1;
    char* copy = (char*)malloc(length);
    memcpy(copy, value, length);
    return copy;
}

static rv_obj* rv_obj_alloc(rv_type type) {
    rv_obj* obj = (rv_obj*)calloc(1, sizeof(rv_obj));
    obj->t = type;
    rv_state_track(obj);
    return obj;
}

rv_obj* rv_integer_new(int v) {
    rv_obj* obj = rv_obj_alloc(RV_TYPE_INTEGER);
    obj->integer_v = v;
    return obj;
}

rv_obj* rv_real_new(double v) {
    rv_obj* obj = rv_obj_alloc(RV_TYPE_REAL);
    obj->real_v = v;
    return obj;
}

rv_obj* rv_bool_new(bool v) {
    rv_obj* obj = rv_obj_alloc(RV_TYPE_BOOL);
    obj->bool_v = v;
    return obj;
}

rv_obj* rv_symbol_new(const char* v) {
    rv_obj* obj = rv_obj_alloc(RV_TYPE_SYMBOL);
    obj->string_v = rv_strdup(v);
    return obj;
}

rv_obj* rv_string_new(const char* v) {
    rv_obj* obj = rv_obj_alloc(RV_TYPE_STRING);
    obj->string_v = rv_strdup(v);
    return obj;
}

rv_obj* rv_pair_new(rv_obj* car, rv_obj* cdr) {
    rv_obj* obj = rv_obj_alloc(RV_TYPE_PAIR);
    rv_pair pair = {car, cdr};
    obj->pair_v = pair;
    return obj;
}

rv_obj* rv_nil_new() {
    rv_obj* obj = rv_obj_alloc(RV_TYPE_NIL);
    obj->integer_v = 0;
    return obj;
}

rv_obj* rv_cfunc_new(const char* func_name, rv_cfunc func) {
    (void)func_name;
    rv_obj* obj = rv_obj_alloc(RV_TYPE_CFUNC);
    obj->cfunc_v = func;
    return obj;
}

rv_obj* rv_pair_get_car(rv_obj* pair) {
    assert(pair->t == RV_TYPE_PAIR);
    return pair->pair_v.car;
}

rv_obj* rv_pair_get_cdr(rv_obj* pair) {
    assert(pair->t == RV_TYPE_PAIR);
    return pair->pair_v.cdr;
}

rv_obj* rv_pair_set_car(rv_obj* pair, rv_obj* obj) {
    assert(pair->t == RV_TYPE_PAIR);
    pair->pair_v.car = obj;
    return pair;
}

rv_obj* rv_pair_set_cdr(rv_obj* pair, rv_obj* obj) {
    assert(pair->t == RV_TYPE_PAIR);
    pair->pair_v.cdr = obj;
    return pair;
}

static char* rv_format(const char* format, ...) {
    va_list args;
    va_start(args, format);
    va_list copy;
    va_copy(copy, args);
    int length = vsnprintf(NULL, 0, format, copy);
    va_end(copy);
    char* result = (char*)malloc((size_t)length + 1);
    vsnprintf(result, (size_t)length + 1, format, args);
    va_end(args);
    return result;
}

char* rv_obj_to_string(const rv_obj* obj) {
    if (obj == NULL) {
        return rv_strdup("<null>");
    }
    switch (obj->t) {
    case RV_TYPE_INTEGER:
        return rv_format("%d", obj->integer_v);
    case RV_TYPE_REAL:
        return rv_format("%f", obj->real_v);
    case RV_TYPE_BOOL:
        return rv_strdup(obj->bool_v ? "#t" : "#f");
    case RV_TYPE_SYMBOL:
        return rv_format("'%s", obj->string_v);
    case RV_TYPE_STRING:
        return rv_format("\"%s\"", obj->string_v);
    case RV_TYPE_PAIR: {
        if (obj->pair_v.car == NULL && obj->pair_v.cdr == NULL) {
            return rv_strdup("( . )");
        }
        char* car = rv_obj_to_string(obj->pair_v.car);
        char* cdr = rv_obj_to_string(obj->pair_v.cdr);
        char* result = rv_format("(%s . %s)", car, cdr);
        free(car);
        free(cdr);
        return result;
    }
    case RV_TYPE_NIL:
        return rv_strdup("()");
    case RV_TYPE_CFUNC:
        return rv_strdup("<cfunc>");
    default:
        return rv_strdup("<object>");
    }
}

void rv_obj_print(rv_obj* obj) {
    char* value = rv_obj_to_string(obj);
    fputs(value, stdout);
    free(value);
}

static bool rv_nullable_equal(const rv_obj* a, const rv_obj* b) {
    if (a == NULL || b == NULL) {
        return a == b;
    }
    return rv_obj_equal(a, b);
}

bool rv_obj_equal(const rv_obj* a, const rv_obj* b) {
    if (a == NULL || b == NULL || a->t != b->t) {
        return a == b;
    }
    switch (a->t) {
    case RV_TYPE_INTEGER:
        return a->integer_v == b->integer_v;
    case RV_TYPE_REAL:
        return a->real_v == b->real_v;
    case RV_TYPE_BOOL:
        return a->bool_v == b->bool_v;
    case RV_TYPE_SYMBOL:
    case RV_TYPE_STRING:
        return strcmp(a->string_v, b->string_v) == 0;
    case RV_TYPE_PAIR:
        return rv_nullable_equal(a->pair_v.car, b->pair_v.car) &&
               rv_nullable_equal(a->pair_v.cdr, b->pair_v.cdr);
    case RV_TYPE_NIL:
        return true;
    case RV_TYPE_CFUNC:
        return a->cfunc_v == b->cfunc_v;
    default:
        return false;
    }
}

void rv_obj_free(rv_obj* obj) {
    if (obj == NULL) {
        return;
    }
    rv_state_untrack(obj);
    if (obj->t == RV_TYPE_SYMBOL || obj->t == RV_TYPE_STRING) {
        free(obj->string_v);
    }
    free(obj);
}

bool rv_obj_is_nil(rv_obj* obj) {
    return (obj->t == RV_TYPE_PAIR)
           && (obj->pair_v.car == NULL)
           && (obj->pair_v.cdr == NULL)
           ;
}

bool rv_obj_is_procedure(rv_obj* obj) {
    return (obj->t == RV_TYPE_PAIR)
           && (obj->pair_v.car->t == RV_TYPE_SYMBOL)
           && (strcmp(obj->pair_v.car->string_v, "lambda") == 0)
           ;
}
