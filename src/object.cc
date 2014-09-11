#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "object.h"

using namespace rv;

const char* object::Object::ToString() {
    return "not implemented";
}

object::type::type object::Object::GetType() {
    return this->type;
}

bool object::Object::EqualTo(Object *that) {
    if (this->GetType() != that->GetType()) {
        return false;
    }
    switch (this->GetType()) {
    case object::type::INTEGER: {
        auto thisInteger = static_cast<object::Integer *>(this);
        auto thatInteger = static_cast<object::Integer *>(that);
        return thisInteger->GetValue() == thatInteger->GetValue();
        break;
    }
    case object::type::REAL: {
        auto thisReal = static_cast<object::Real *>(this);
        auto thatReal = static_cast<object::Real *>(that);
        return thisReal->GetValue() == thatReal->GetValue();
    }
    case object::type::BOOL: {
        auto thisBool = static_cast<object::Bool *>(this);
        auto thatBool = static_cast<object::Bool *>(that);
        return thisBool->GetValue() == thatBool->GetValue();
    }
    case object::type::PAIR: {
        auto thisPair = static_cast<object::Pair *>(this);
        auto thatPair = static_cast<object::Pair *>(this);
        if (thisPair->GetCar() == nullptr &&
            thisPair->GetCdr() == nullptr &&
            thatPair->GetCar() == nullptr &&
            thatPair->GetCdr() == nullptr) {
            return true;
        }

        if (thisPair->GetCar() == nullptr && thatPair->GetCdr() != nullptr) {
            return false;
        }
        if (! thisPair->GetCar()->EqualTo(thatPair->GetCar())) {
            return false;
        }

        if (thatPair->GetCar() == nullptr && thatPair->GetCdr() != nullptr) {
            return false;
        }
        if (! thisPair->GetCdr()->EqualTo(thatPair->GetCdr())) {
            return false;
        }
        return true;
    }
    case object::type::STRING: {
        auto thisString = static_cast<object::String *>(this);
        auto thatString = static_cast<object::String *>(that);
        if (strcmp(thisString->GetValue(), thatString->GetValue()) == 0) {
            return true;
        } else {
            return false;
        }
    }
    case object::type::SYMBOL: {
        auto thisSymbol = static_cast<object::Symbol *>(this);
        auto thatSymbol = static_cast<object::Symbol *>(that);
        if (strcmp(thisSymbol->GetValue(), thatSymbol->GetValue()) == 0) {
            return true;
        } else {
            return false;
        }
    }
    default: {
        fprintf(stderr, "Not implemented!\n");
        return false;
    }
    }
}

object::Integer::Integer(long value) {
    this->type = object::type::INTEGER;
    this->value = value;
}

const char* object::Integer::ToString() {
    auto buffer = new char[12]();
    snprintf(buffer, 12, "%ld", this->value);
    return buffer;
}

object::Real::Real(double value) {
    this->type = object::type::REAL;
    this->value = value;
}

const char* object::Real::ToString() {
    auto buffer = new char[12]();
    snprintf(buffer, 12, "%f", this->value);
    return buffer;
}

object::Bool::Bool(bool value) {
    this->type = object::type::BOOL;
    this->value = value;
}

const char *object::Bool::ToString() {
    if (this->value) {
        return new char[3]{'#', 't', 0};
    } else {
        return new char[3]{'#', 'f', 0};
    }
}

object::Pair::Pair(Object *car, Object *cdr) {
    this->type = object::type::PAIR;
    this->car = car;
    this->cdr = cdr;
}

const char *object::Pair::ToString() {
    const char *car_str;
    if (this->car == nullptr) {
        car_str = new char[1]{0};
    } else {
        car_str = this->car->ToString();
    }
    size_t car_str_length = strlen(car_str);

    const char *cdr_str;
    if (this->cdr == nullptr) {
        cdr_str = new char[1]{0};
    } else {
        cdr_str = this->cdr->ToString();
    }
    size_t cdr_str_length = strlen(cdr_str);

    size_t length = car_str_length + cdr_str_length + 6;
    char* buffer = new char[length]();
    snprintf(buffer, length, "(%s . %s)", car_str, cdr_str);
    delete car_str;
    delete cdr_str;
    return buffer;
}

object::Symbol::Symbol(char *value) {
    this->type = object::type::SYMBOL;
    this->value = value;
}

const char* object::Symbol::ToString() {
    size_t length = strlen(this->value) + 2;
    auto buffer = new char[length]();
    snprintf(buffer, length, "'%s", this->value);
    return buffer;
}

object::String::String(char *value) {
    this->type = object::type::STRING;
    this->value = value;
}

const char* object::String::ToString() {
    size_t length = strlen(this->value) + 3;
    auto buffer = new char[length]();
    snprintf(buffer, length, "\"%s\"", this->value);
    return buffer;
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
