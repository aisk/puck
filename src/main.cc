#include <stdio.h>
#include <stdlib.h>

#include "env.h"
#include "eval.h"
#include "object.h"
#include "parser.h"
#include "state.h"

static rv_obj* rv_cfunc_add(rv_obj* params) {
    (void)params;
    return rv_integer_new(123);
}

int main() {
    char buffer[256];
    rv_state* state = rv_state_new();
    rv_state_set_current(state);
    rv_env* global_env = rv_env_new(NULL);
    rv_env_set(global_env,
               rv_symbol_new("+"),
               rv_cfunc_new("+", rv_cfunc_add));

    puts("Welcome to puck.");
    while (true) {
        printf(">> ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        rv_parser* parser = rv_parser_new(buffer);
        rv_obj* obj = rv_parse(parser);
        rv_parser_free(parser);

        rv_obj* result = rv_eval(obj, global_env);
        if (result != NULL) {
            printf("=> ");
            rv_obj_print(result);
            printf("\n");
        }
    }
    rv_env_free(global_env);
    rv_state_free(state);
    return 0;
}
