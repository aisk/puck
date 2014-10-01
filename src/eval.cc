#include <stdio.h>
#include <string.h>

#include "object.h"
#include "env.h"
#include "eval.h"

using namespace puck;

static void debug(const char* s) {
    printf("[DEBUG] %s\n", s);
}

object::Object *puck::Eval(object::Object &obj, Env &env) {
    switch (obj.GetType()) {
    case object::type::SYMBOL:
    {
        debug("eval symbol");
        return env.Get(obj);
    }
    case object::type::INTEGER:
    case object::type::REAL:
    case object::type::BOOL:
    case object::type::CHAR:
    case object::type::STRING:
    case object::type::VECTOR:
    case object::type::NIL:
    {
        debug("eval literal");
        return &obj;
    }

    }
    return nullptr;
}

rv_obj* rv_eval(rv_obj* obj, rv_env* env) {
    rv_obj* car;
    rv_obj* cdr;
    switch (obj->t) {
    case RV_TYPE_SYMBOL:
        debug("eval symbol");
        return rv_env_get(env, obj);
    case RV_TYPE_NUMBER:
    case RV_TYPE_INTEGER:
    case RV_TYPE_REAL:
    case RV_TYPE_BOOL:
    case RV_TYPE_CHAR:
    case RV_TYPE_STRING:
    case RV_TYPE_VECTOR:
    case RV_TYPE_NIL:
        debug("eval literal");
        return obj;
    case RV_TYPE_PAIR:
        debug("eval pair");
        car = obj->pair_v.car;
        cdr = obj->pair_v.cdr;
        if (car->t == RV_TYPE_SYMBOL && strcmp(car->string_v, "quote") == 0) {
            debug("eval quote");
            return cdr;
        }
        if (car->t == RV_TYPE_SYMBOL && strcmp(car->string_v, "if") == 0) {
            debug("eval if");
        } else if (car->t == RV_TYPE_SYMBOL && strcmp(car->string_v, "define") == 0) {
            debug("eval define");
            rv_obj* var = cdr->pair_v.car;
            rv_obj* exp = cdr->pair_v.cdr->pair_v.car;
            rv_env_set(env, var, rv_eval(exp, env));
            return var;
        } else if (car->t == RV_TYPE_SYMBOL && strcmp(car->string_v, "lambda") == 0) {
            debug("eval lambda");
            // rv_obj* params = cdr->pair_v.car;
            // rv_obj* body = cdr->pair_v.cdr->pair_v.car;
            return obj;
        } else if (car->t == RV_TYPE_SYMBOL && strcmp(car->string_v, "begin") == 0) {
            debug("eval begin");
            // TODO: eval every rest and return last value
            return obj;
        } else if (car->t == RV_TYPE_CFUNC) {
            debug("eval cfunc");
            rv_cfunc func = car->cfunc_v;
            return func(cdr);
        } else {
            debug("proc exp*");
            rv_obj* head = rv_eval(car, env);
            rv_obj* iter;
            for (iter = obj; !rv_obj_is_nil(iter); iter = iter->pair_v.cdr) {
                rv_obj* var = rv_eval(iter->pair_v.car, env);
                if (var == NULL) {
                    printf("unbound variable ");
                    rv_obj_print(iter->pair_v.car);
                    printf(" !\n");
                    exit(1);
                }
                rv_obj_print(var);
            }
            return obj;
        }
        return NULL;
    default:
        debug("not implemented");
    }
    return NULL;
}

