#include <stdio.h>
#include <string.h>

#include "env.h"
#include "eval.h"
#include "object.h"

static bool is_empty(const rv_obj* obj) {
    return obj != NULL && obj->t == RV_TYPE_PAIR &&
           obj->pair_v.car == NULL && obj->pair_v.cdr == NULL;
}

static rv_obj* first(rv_obj* list) {
    return list == NULL || is_empty(list) ? NULL : list->pair_v.car;
}

static rv_obj* rest(rv_obj* list) {
    return list == NULL || is_empty(list) ? NULL : list->pair_v.cdr;
}

static rv_obj* second(rv_obj* list) {
    return first(rest(list));
}

static rv_obj* third(rv_obj* list) {
    return first(rest(rest(list)));
}

static bool is_truthy(const rv_obj* obj) {
    return obj != NULL && !(obj->t == RV_TYPE_BOOL && !obj->bool_v) && !is_empty(obj);
}

static rv_obj* eval_sequence(rv_obj* expressions, rv_env* env) {
    rv_obj* result = rv_bool_new(false);
    for (rv_obj* iter = expressions; iter != NULL && !is_empty(iter); iter = rest(iter)) {
        result = rv_eval(first(iter), env);
        if (result == NULL) {
            return NULL;
        }
    }
    return result;
}

static rv_obj* eval_args(rv_obj* expressions, rv_env* env) {
    rv_obj* head = rv_pair_new(NULL, NULL);
    rv_obj* tail = head;
    for (rv_obj* iter = expressions; iter != NULL && !is_empty(iter); iter = rest(iter)) {
        rv_obj* value = rv_eval(first(iter), env);
        if (value == NULL) {
            return NULL;
        }
        tail->pair_v.car = value;
        tail->pair_v.cdr = rv_pair_new(NULL, NULL);
        tail = tail->pair_v.cdr;
    }
    return head;
}

static rv_obj* apply_function(rv_obj* function, rv_obj* args, rv_env* env) {
    if (function->t == RV_TYPE_CFUNC) {
        return function->cfunc_v(args);
    }
    if (function->t != RV_TYPE_FUNCTION) {
        fprintf(stderr, "value is not callable\n");
        return NULL;
    }

    rv_env* local = rv_env_new(env);
    rv_obj* params = function->function_v.params;
    rv_obj* values = args;
    while (!is_empty(params) && !is_empty(values)) {
        rv_obj* name = first(params);
        if (name == NULL || name->t != RV_TYPE_SYMBOL) {
            fprintf(stderr, "function parameter must be a symbol\n");
            rv_env_free(local);
            return NULL;
        }
        rv_env_set(local, name, first(values));
        params = rest(params);
        values = rest(values);
    }
    if (!is_empty(params) || !is_empty(values)) {
        fprintf(stderr, "wrong number of arguments\n");
        rv_env_free(local);
        return NULL;
    }

    rv_obj* result = eval_sequence(function->function_v.body, local);
    rv_env_free(local);
    return result;
}

rv_obj* rv_eval(rv_obj* obj, rv_env* env) {
    if (obj == NULL) {
        return NULL;
    }
    if (obj->t == RV_TYPE_SYMBOL) {
        rv_obj* value = rv_env_get(env, obj);
        if (value == NULL) {
            fprintf(stderr, "undefined name: %s\n", obj->string_v);
        }
        return value;
    }
    if (obj->t != RV_TYPE_PAIR || is_empty(obj)) {
        return obj;
    }

    rv_obj* op = first(obj);
    rv_obj* operands = rest(obj);
    if (op != NULL && op->t == RV_TYPE_SYMBOL) {
        const char* name = op->string_v;
        if (strcmp(name, "quote") == 0) {
            return first(operands);
        }
        if (strcmp(name, "if") == 0) {
            rv_obj* condition = rv_eval(first(operands), env);
            return rv_eval(is_truthy(condition) ? second(operands) : third(operands), env);
        }
        if (strcmp(name, "begin") == 0) {
            return eval_sequence(operands, env);
        }
        if (strcmp(name, "fn") == 0) {
            rv_obj* params = first(operands);
            rv_obj* body = rest(operands);
            if (params == NULL || params->t != RV_TYPE_PAIR || is_empty(body)) {
                fprintf(stderr, "fn expects a parameter list and a body\n");
                return NULL;
            }
            return rv_function_new(params, body);
        }
        if (strcmp(name, "define") == 0) {
            rv_obj* target = first(operands);
            rv_obj* value;
            rv_obj* variable;
            if (target != NULL && target->t == RV_TYPE_PAIR && !is_empty(target)) {
                variable = first(target);
                value = rv_function_new(rest(target), rest(operands));
            } else {
                variable = target;
                value = rv_eval(second(operands), env);
            }
            if (variable == NULL || variable->t != RV_TYPE_SYMBOL || value == NULL) {
                fprintf(stderr, "define expects a name and a value\n");
                return NULL;
            }
            rv_env_set(env, variable, value);
            return value;
        }
    }

    rv_obj* function = rv_eval(op, env);
    if (function == NULL) {
        return NULL;
    }
    rv_obj* args = eval_args(operands, env);
    if (args == NULL) {
        return NULL;
    }
    return apply_function(function, args, env);
}

static bool number_value(rv_obj* obj, double* value, bool* is_real) {
    if (obj->t == RV_TYPE_INTEGER) {
        *value = obj->integer_v;
        return true;
    }
    if (obj->t == RV_TYPE_REAL) {
        *value = obj->real_v;
        *is_real = true;
        return true;
    }
    fprintf(stderr, "expected a number\n");
    return false;
}

static rv_obj* numeric_result(double value, bool real) {
    return real ? rv_real_new(value) : rv_integer_new((int)value);
}

static rv_obj* builtin_add(rv_obj* args) {
    double result = 0;
    bool real = false;
    for (rv_obj* iter = args; !is_empty(iter); iter = rest(iter)) {
        double value;
        if (!number_value(first(iter), &value, &real)) return NULL;
        result += value;
    }
    return numeric_result(result, real);
}

static rv_obj* builtin_multiply(rv_obj* args) {
    double result = 1;
    bool real = false;
    for (rv_obj* iter = args; !is_empty(iter); iter = rest(iter)) {
        double value;
        if (!number_value(first(iter), &value, &real)) return NULL;
        result *= value;
    }
    return numeric_result(result, real);
}

static rv_obj* builtin_subtract(rv_obj* args) {
    if (is_empty(args)) return NULL;
    bool real = false;
    double result;
    if (!number_value(first(args), &result, &real)) return NULL;
    rv_obj* iter = rest(args);
    if (is_empty(iter)) return numeric_result(-result, real);
    for (; !is_empty(iter); iter = rest(iter)) {
        double value;
        if (!number_value(first(iter), &value, &real)) return NULL;
        result -= value;
    }
    return numeric_result(result, real);
}

static rv_obj* builtin_divide(rv_obj* args) {
    if (is_empty(args) || is_empty(rest(args))) return NULL;
    bool real = true;
    double result;
    if (!number_value(first(args), &result, &real)) return NULL;
    for (rv_obj* iter = rest(args); !is_empty(iter); iter = rest(iter)) {
        double value;
        if (!number_value(first(iter), &value, &real)) return NULL;
        if (value == 0) {
            fprintf(stderr, "division by zero\n");
            return NULL;
        }
        result /= value;
    }
    return rv_real_new(result);
}

static rv_obj* builtin_equal(rv_obj* args) {
    if (is_empty(args) || is_empty(rest(args))) return rv_bool_new(true);
    rv_obj* expected = first(args);
    for (rv_obj* iter = rest(args); !is_empty(iter); iter = rest(iter)) {
        if (!rv_obj_equal(expected, first(iter))) return rv_bool_new(false);
    }
    return rv_bool_new(true);
}

static rv_obj* compare_numbers(rv_obj* args, int direction) {
    if (is_empty(args) || is_empty(rest(args))) return rv_bool_new(true);
    bool real = false;
    double previous;
    if (!number_value(first(args), &previous, &real)) return NULL;
    for (rv_obj* iter = rest(args); !is_empty(iter); iter = rest(iter)) {
        double current;
        if (!number_value(first(iter), &current, &real)) return NULL;
        if ((direction < 0 && !(previous < current)) ||
            (direction > 0 && !(previous > current))) return rv_bool_new(false);
        previous = current;
    }
    return rv_bool_new(true);
}

static rv_obj* builtin_less(rv_obj* args) { return compare_numbers(args, -1); }
static rv_obj* builtin_greater(rv_obj* args) { return compare_numbers(args, 1); }
static rv_obj* builtin_not(rv_obj* args) { return rv_bool_new(!is_truthy(first(args))); }
static rv_obj* builtin_list(rv_obj* args) { return args; }

static rv_obj* builtin_cons(rv_obj* args) {
    return rv_pair_new(first(args), second(args));
}

static rv_obj* builtin_car(rv_obj* args) {
    rv_obj* pair = first(args);
    return pair != NULL && pair->t == RV_TYPE_PAIR ? pair->pair_v.car : NULL;
}

static rv_obj* builtin_cdr(rv_obj* args) {
    rv_obj* pair = first(args);
    return pair != NULL && pair->t == RV_TYPE_PAIR ? pair->pair_v.cdr : NULL;
}

static rv_obj* builtin_print(rv_obj* args) {
    rv_obj* result = rv_bool_new(false);
    bool needs_space = false;
    for (rv_obj* iter = args; !is_empty(iter); iter = rest(iter)) {
        if (needs_space) putchar(' ');
        result = first(iter);
        rv_obj_print(result);
        needs_space = true;
    }
    putchar('\n');
    return result;
}

static void add_builtin(rv_env* env, const char* name, rv_cfunc function) {
    rv_env_set(env, rv_symbol_new(name), rv_cfunc_new(name, function));
}

void rv_env_add_builtins(rv_env* env) {
    add_builtin(env, "+", builtin_add);
    add_builtin(env, "-", builtin_subtract);
    add_builtin(env, "*", builtin_multiply);
    add_builtin(env, "/", builtin_divide);
    add_builtin(env, "=", builtin_equal);
    add_builtin(env, "<", builtin_less);
    add_builtin(env, ">", builtin_greater);
    add_builtin(env, "not", builtin_not);
    add_builtin(env, "list", builtin_list);
    add_builtin(env, "cons", builtin_cons);
    add_builtin(env, "car", builtin_car);
    add_builtin(env, "cdr", builtin_cdr);
    add_builtin(env, "print", builtin_print);
}
