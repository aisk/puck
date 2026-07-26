#ifndef RV_PARSER_H
#define RV_PARSER_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "object.h"

typedef struct rv_parser {
    const char* src;
    size_t pos;
    size_t line;
} rv_parser;

rv_parser* rv_parser_new(const char* src);
void rv_parser_free(rv_parser* parser);
rv_obj* rv_parse(rv_parser* parser);

#endif
