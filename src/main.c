#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env.h"
#include "eval.h"
#include "object.h"
#include "parser.h"
#include "state.h"

static int run_source(const char* source, rv_env* env, bool print_results) {
    rv_parser* parser = rv_parser_new(source);
    while (rv_parser_has_more(parser)) {
        rv_obj* expression = rv_parse(parser);
        if (expression == NULL) {
            fprintf(stderr, "%zu:%zu: %s\n",
                    rv_parser_error_line(parser),
                    rv_parser_error_column(parser),
                    rv_parser_error(parser));
            rv_parser_free(parser);
            return 1;
        }
        rv_obj* result = rv_eval(expression, env);
        if (result == NULL) {
            rv_parser_free(parser);
            return 1;
        }
        if (print_results) {
            printf("=> ");
            rv_obj_print(result);
            putchar('\n');
        }
    }
    rv_parser_free(parser);
    return 0;
}

static int paren_balance(const char* source) {
    int balance = 0;
    bool in_string = false;
    bool escaped = false;
    bool comment = false;
    for (const char* cursor = source; *cursor != 0; cursor++) {
        char c = *cursor;
        if (comment) {
            if (c == '\n') comment = false;
            continue;
        }
        if (in_string) {
            if (escaped) escaped = false;
            else if (c == '\\') escaped = true;
            else if (c == '"') in_string = false;
            continue;
        }
        if (c == ';') comment = true;
        else if (c == '"') in_string = true;
        else if (c == '(') balance++;
        else if (c == ')') balance--;
    }
    return balance;
}

static int run_repl(rv_env* env) {
    char line[1024];
    char source[16384];
    source[0] = 0;
    puts("puck tiny Lisp. Press Ctrl-D to exit.");
    while (true) {
        printf(source[0] == 0 ? ">> " : ".. ");
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        if (strlen(source) + strlen(line) >= sizeof(source)) {
            fprintf(stderr, "expression is too large\n");
            source[0] = 0;
            continue;
        }
        strcat(source, line);
        int balance = paren_balance(source);
        if (balance > 0) continue;
        if (balance < 0) {
            fprintf(stderr, "unexpected ')'\n");
        } else {
            run_source(source, env, true);
        }
        source[0] = 0;
    }
    return 0;
}

static char* read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        perror(path);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);
    char* source = (char*)malloc((size_t)length + 1);
    size_t read = fread(source, 1, (size_t)length, file);
    source[read] = 0;
    fclose(file);
    return source;
}

int main(int argc, char** argv) {
    rv_state* state = rv_state_new();
    rv_state_set_current(state);
    rv_env* global_env = rv_env_new(NULL);
    rv_env_add_builtins(global_env);

    int result = 0;
    if (argc == 1) {
        result = run_repl(global_env);
    } else if (argc == 2) {
        char* source = read_file(argv[1]);
        result = source == NULL ? 1 : run_source(source, global_env, false);
        free(source);
    } else {
        fprintf(stderr, "usage: %s [file]\n", argv[0]);
        result = 1;
    }

    rv_env_free(global_env);
    rv_state_free(state);
    return result;
}
