#ifndef RV_PARSER_H
#define RV_PARSER_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "object.h"

typedef struct rv_parser {
    char* src;
    size_t pos;
    size_t line;
} rv_parser;

rv_parser* rv_parser_new(char* src);
rv_obj* rv_parse(rv_parser* parser);

#endif
