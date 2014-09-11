#ifndef RV_OBJECT_H
#define RV_OBJECT_H

#include <stdlib.h>
#include <stdbool.h>

namespace rv {
namespace object {
    namespace type {
        enum type {
            INVALID,
            INTEGER,
            REAL,
            BOOL,
            PAIR,
            LIST,
            SYMBOL,
            CHAR,
            STRING,
            VECTOR,
            NIL,
            CFUNC,
        };
    }
    class Object {
    protected:
        type::type type;
    public:
        virtual type::type GetType();
        virtual const char *ToString();
        bool EqualTo(Object*);
    };

    class Number : public Object {
    };

    class Integer : public Number {
        long value;
    public:
        Integer(long);
        long GetValue() { return this->value; };
        const char *ToString();
    };

    class Real : public Number {
        double value;
    public:
        Real(double);
        double GetValue() { return this->value; };
        const char *ToString();
    };

    class Bool : public Object {
        bool value;
    public:
        Bool(bool);
        bool GetValue() { return this->value; };
        const char *ToString();
    };

    class Pair : public Object {
        Object *car;
        Object *cdr;
    public:
        Pair(Object *, Object *);
        bool IsNil() { return (this->car == nullptr) && (this->cdr == nullptr); }
        Object *GetCar() { return this->car; }
        Object *GetCdr() { return this->cdr; }
        void SetCar(Object *car) { this->car = car; }
        void SetCdr(Object *cdr) { this->cdr = cdr; }
        const char*ToString();
    };

    class List : public Pair {

    };

    class Symbol : public Object {    // FIXME: now symbol implemention is just a char array;
        char *value;
    public:
        Symbol(char *);
        char *GetValue() { return this->value; };
        const char *ToString();
    };

    class Char : public Object {

    };

    class String : public Object {
        char *value;
    public:
        String(char *);
        char *GetValue() { return this->value; };
        const char *ToString();
    };

    class Vector : public Object {

    };

    class Nil : public Object {

    };

    class Cfunc : public Object {

    };


} // namespace object
} // namespace rv

typedef enum {
    RV_TYPE_NUMBER = 0,
    RV_TYPE_INTEGER,
    RV_TYPE_REAL,
    RV_TYPE_BOOL,
    RV_TYPE_PAIR,
    RV_TYPE_LIST,
    RV_TYPE_SYMBOL,
    RV_TYPE_CHAR,
    RV_TYPE_STRING,
    RV_TYPE_VECTOR,
    RV_TYPE_NIL,
    RV_TYPE_CFUNC,
} rv_type;

struct rv_pair;
struct rv_obj;
typedef struct rv_pair rv_pair;
typedef struct rv_obj rv_obj;
typedef rv_obj* (*rv_cfunc)(rv_obj*);

struct rv_pair {
    rv_obj* car;
    rv_obj* cdr;
};

struct rv_obj {
    rv_type t;
    union {
        int integer_v;
        double real_v;
        bool bool_v;
        char* string_v;
        rv_pair pair_v;
        rv_cfunc cfunc_v;
    };
};

rv_obj* rv_integer_new(int v);
rv_obj* rv_real_new(double v);
rv_obj* rv_bool_new(bool v);
rv_obj* rv_symbol_new(char* v);
rv_obj* rv_string_new(char* v);
rv_obj* rv_pair_new(rv_obj* car, rv_obj* cdr);
rv_obj* rv_nil_new();
rv_obj* rv_cfunc_new(char* func_name, rv_cfunc func);

rv_obj* rv_pair_get_car(rv_obj* obj);
rv_obj* rv_pair_get_cdr(rv_obj* obj);
rv_obj* rv_pair_set_car(rv_obj* pair, rv_obj* obj);
rv_obj* rv_pair_set_cdr(rv_obj* pair, rv_obj* obj);

void rv_obj_print(rv_obj* obj);

void rv_obj_free(rv_obj* obj);

bool rv_obj_is_nil(rv_obj* obj);

#endif
