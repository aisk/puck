#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "object.h"

const char* rv::object::Object::ToString() {
    return "not implemented";
}

rv::object::type::type rv::object::Object::GetType() {
    return this->type;
}

rv::object::Integer::Integer(long value) {
    this->type = rv::object::type::INTEGER;
    this->value = value;
}

const char* rv::object::Integer::ToString() {
    auto buffer = new char[12]();
    snprintf(buffer, 12, "%ld", this->value);
    return buffer;
}

rv::object::Real::Real(double value) {
    this->type = rv::object::type::REAL;
    this->value = value;
}

rv::object::Bool::Bool(bool value) {
    this->type = rv::object::type::BOOL;
    this->value = value;
}


rv_obj* rv_integer_new(int v) {
    rv_obj* obj = (rv_obj*)malloc(sizeof(rv_obj));
    obj->t = RV_TYPE_INTEGER;
    obj->integer_v = v;
    return obj;
}

rv_obj* rv_real_new(double v) {
    rv_obj* obj = (rv_obj*)malloc(sizeof(rv_obj));
    obj->t = RV_TYPE_REAL;
    obj->real_v = v;
    return obj;
}

rv_obj* rv_bool_new(bool v) {
    rv_obj* obj = (rv_obj*)malloc(sizeof(rv_obj));
    obj->t = RV_TYPE_BOOL;
    obj->bool_v = v;
    return obj;
}

rv_obj* rv_symbol_new(char* v) {
    rv_obj* obj = (rv_obj*)malloc(sizeof(rv_obj));
    obj->t = RV_TYPE_SYMBOL;
    obj->string_v = v;
    return obj;
}

rv_obj* rv_string_new(char* v) {
    rv_obj* obj = (rv_obj*)malloc(sizeof(rv_obj));
    obj->t = RV_TYPE_STRING;
    obj->string_v = v;
    return obj;
}

rv_obj* rv_pair_new(rv_obj* car, rv_obj* cdr) {
    rv_obj* obj = (rv_obj*)malloc(sizeof(rv_obj));
    rv_pair pair = {car, cdr};
    obj->t = RV_TYPE_PAIR;
    obj->pair_v = pair;
    return obj;
}

rv_obj* rv_nil_new() {
    rv_obj* obj = (rv_obj*)malloc(sizeof(rv_obj));
    obj->t = RV_TYPE_NIL;
    obj->integer_v = 0;
    return obj;
}

rv_obj* rv_cfunc_new(char* func_name, rv_cfunc func) {
    rv_obj* obj = (rv_obj*)malloc(sizeof(rv_obj));
    obj->t = RV_TYPE_CFUNC;
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
    obj->pair_v.car = obj;
    assert(obj->pair_v.car == obj);
    return pair;
}

rv_obj* rv_pair_set_cdr(rv_obj* pair, rv_obj* obj) {
    assert(obj->t == RV_TYPE_PAIR);
    obj->pair_v.cdr = obj;
    return obj;
}

void rv_obj_print(rv_obj* obj) {
    switch (obj->t) {
    case RV_TYPE_INTEGER:
        printf("<%d", obj->integer_v);
        printf(":integer@%p>", obj);
        break;
    case RV_TYPE_REAL:
        printf("<%f", obj->real_v);
        printf(":real@%p>", obj);
        break;
    case RV_TYPE_BOOL:
        if (obj->bool_v) {
            printf("<#t");
            printf(":bool@%p>", obj);
        } else {
            printf("<#f");
            printf(":bool@%p>", obj);
        }
        break;
    case RV_TYPE_SYMBOL:
        printf("<%s", obj->string_v);
        printf(":symbol@%p>", obj);
        break;
    case RV_TYPE_STRING:
        printf("<%s", obj->string_v);
        printf(":string@%p>", obj);
        break;
    case RV_TYPE_PAIR:
        if (obj->pair_v.car == NULL && obj->pair_v.cdr == NULL) {
            printf("()");
            return;
        }
        printf("(");
        rv_obj_print(obj->pair_v.car);
        printf(" . ");
        rv_obj_print(obj->pair_v.cdr);
        printf(")");
        break;
    case RV_TYPE_CFUNC:
        printf("<cfunc@%p>", obj);
        break;
    default:
        puts("obj print not implemented");
        exit(1);
    }
}

void rv_obj_free(rv_obj* obj) {
    switch (obj->t) {
    case RV_TYPE_INTEGER:
    case RV_TYPE_REAL:
    case RV_TYPE_BOOL:
        free(obj);
        break;
    case RV_TYPE_SYMBOL:
        free(obj->string_v);
        free(obj);
        break;
    case RV_TYPE_PAIR:
        free(obj->pair_v.car);
        free(obj->pair_v.cdr);
        free(obj);
        break;
    default:
        puts("not implemented");
        exit(1);
    }
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
