#include <stdio.h>
#include <stdbool.h>

#include "parser.h"
#include "eval.h"
#include "object.h"
#include "state.h"

using namespace puck;

int main() {
    puts("Welcome to rivai.");
    char buffer[256];
    InitGlobalState();
    Env root_env;

    while (true) {
        printf(">> ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        // puts(buffer);

        auto parser = Parser(buffer);
        auto obj = parser.ParseExpr();
        printf("parse => %s\n", obj->ToString());

        auto ret = puck::Eval(*obj, root_env);
        printf("eval => %s\n", ret->ToString());
    }

    DestroyGlobalState();
}

rv_obj* rv_cfunc_add(rv_obj* params) {
    return rv_integer_new(123);
}

// int main() {
//     char buffer[256];
//     rv_env* global_env = rv_env_new(NULL);
//     rv_env_set(global_env, rv_string_new(const_cast<char *>("+")), rv_cfunc_new(const_cast<char *>("+"), rv_cfunc_add));
//     while (true) {
//         printf(">> ");
//         if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
//             break;
//         }
//         rv_parser* parser = rv_parser_new(buffer);
//         // printf("[PARSE] ");
//         rv_obj* obj = rv_parse(parser);
//         // rv_obj_print(obj);
//         // printf("\n");
//         free(parser);
//         rv_obj* ret_obj = rv_eval(obj, global_env);
//         if (ret_obj != NULL) {
//             printf("=> ");
//             rv_obj_print(ret_obj);
//             printf("\n");
//         }
//     }
//
//     return 0;
// }
