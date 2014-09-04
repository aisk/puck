#include <stdio.h>
#include <stdbool.h>

#include "parser.h"
#include "eval.h"
#include "object.h"

rv_obj* rv_cfunc_add(rv_obj* params) {
    // puts("hehe");
    return rv_integer_new(123);
}

int main() {
    char buffer[256];
    rv_env* global_env = rv_env_new(NULL);
    rv_env_set(global_env, rv_string_new("+"), rv_cfunc_new("+", rv_cfunc_add));
    while (true) {
        printf(">> ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        rv_parser* parser = rv_parser_new(buffer);
        // printf("[PARSE] ");
        rv_obj* obj = rv_parse(parser);
        // rv_obj_print(obj);
        // printf("\n");
        free(parser);
        rv_obj* ret_obj = rv_eval(obj, global_env);
        if (ret_obj != NULL) {
            printf("=> ");
            rv_obj_print(ret_obj);
            printf("\n");
        }
    }

    return 0;
}
