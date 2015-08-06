#ifndef RV_PARSER_H
#define RV_PARSER_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "object.h"

namespace puck {
class Parser {
    char* src;
    size_t pos;
    size_t line;
    bool HaveMore();
    void Pop(size_t);
    char Peek(size_t);
    bool Eat(char);
    void SkipWhite();
    void SkipComment();
    void SkipAll();
    object::Object *ParseNumber();
    object::Object *ParseList();
    object::Object *ParseBool();
    object::Object *ParseString();
    object::Object *ParseSymbol();
public:
    object::Object *ParseExpr();
    Parser(char*);
};
} // namespace rv

typedef struct rv_parser {
    char* src;
    size_t pos;
    size_t line;
} rv_parser;

rv_parser* rv_parser_new(char* src);
rv_obj* rv_parse(rv_parser* parser);

#endif
