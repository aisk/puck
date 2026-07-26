#ifndef RV_PARSER_H
#define RV_PARSER_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "object.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rv_parser {
    const char* src;
    size_t length;
    size_t pos;
    size_t line;
    size_t error_line;
    size_t error_column;
    char error[256];
} rv_parser;

rv_parser* rv_parser_new(const char* src);
void rv_parser_free(rv_parser* parser);
bool rv_parser_has_more(rv_parser* parser);
rv_obj* rv_parse(rv_parser* parser);
const char* rv_parser_error(const rv_parser* parser);
size_t rv_parser_error_line(const rv_parser* parser);
size_t rv_parser_error_column(const rv_parser* parser);

#ifdef __cplusplus
}
#endif

#endif
