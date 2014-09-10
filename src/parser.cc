#include <ctype.h>
#include <string.h>
#include "parser.h"
#include "object.h"

#include "assert.h"

rv::Parser::Parser(char *src) {
    this->src = src;
}

bool rv::Parser::HaveMore() {
    return this->pos < strlen(this->src);
}

void rv::Parser::Pop(size_t n) {
    this->pos += n;
}

char rv::Parser::Peak(size_t idx) {
    if (this->pos + idx < strlen(this->src)) {
        return this->src[this->pos + idx];
    } else {
        return NULL;
    }
}

void rv::Parser::SkipWhite() {
    while (this->HaveMore()) {
        if (this->Eat('\n')) {
            this->line ++;
        } else if (isspace(this->Peak(0))) {
            this->Pop(1);
        } else {
            break;
        }
    }
}

static rv_obj* parse_expr(rv_parser* parser);

rv_parser* rv_parser_new(char* src) {
    rv_parser* parser;
    parser = (rv_parser*) malloc(sizeof(rv_parser));
    parser->src = src;
    parser->pos = 0;
    parser->line = 1;
    return parser;
}

bool rv::Parser::Eat(char c) {
    if (this->Peak(0) != c) {
        return false;
    } else {
        this->pos ++;
        return true;
    }
}

void rv::Parser::SkipComment() {
    while (this->Eat(';')) {
        while (this->HaveMore() && !this->Eat('\n')) {
            this->Pop(1);
        }
        this->line += 1;
    }
}

void rv::Parser::SkipAll() {
    while (true) {
        this->SkipWhite();
        if (this->Peak(0) == ';') {
            this->SkipComment();
        } else {
            break;
        }
    }
}

rv::object::Object *rv::Parser::ParseExpr() {
    this->SkipAll();
    if (!this->HaveMore()) {
        fprintf(stderr, "Nothing to parse.\n");
        exit(1);
    }
    char c = this->Peak(0);
    switch(c) {
    case '#':
        return this->ParseBool();
    case '(':
        return this->ParseList();
    case '+':
    case '-':
        if (!isdigit(this->Peak(1))) {
            break;
        }
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return this->ParseNumber();
    case '"':
        return this->ParseString();
    }
    return this->ParseSymbol();
}


rv::object::Object *rv::Parser::ParseNumber() {
    int sign = 1;
    bool is_real = false;
    if (this->Eat('-')) {
        sign = -1;
    } else if (this->Eat('+')) {
        sign = 1;
    }
    size_t start_pos = this->pos;
    while (isdigit(this->Peak(0))) {
        this->Pop(1);
    }
    if (this->Eat('.')) {
        is_real = true;
    }
    while (isdigit(this->Peak(0))) {
        this->Pop(1);
    }
    size_t end_pos = this->pos;
    if (start_pos == end_pos) {
        puts("Not enough number to parse");
    }
    if (is_real) {
        double value = atof(this->src + start_pos);
        value *= sign;
        return new rv::object::Real(value);
    } else {
        long value = atoi(this->src + start_pos);
        value *= sign;
        return new rv::object::Integer(value);
    }
}

rv::object::Object *rv::Parser::ParseList() {
    return NULL; // TODO
}

rv::object::Object *rv::Parser::ParseBool() {
    return NULL; // TODO
}

rv::object::Object *rv::Parser::ParseString() {
    return NULL; // TODO
}

rv::object::Object *rv::Parser::ParseSymbol() {
    return NULL; // TODO
}

static bool have_more(rv_parser* parser) {
    return parser->pos < strlen(parser->src);
}

static void pop(rv_parser* parser, size_t n) {
    parser->pos += n;
}

static char peak(rv_parser* parser, size_t idx) {
    if (parser->pos + idx < strlen(parser->src)) {
        return parser->src[parser->pos + idx];
    } else {
        return 0;
    }
}

static bool eat(rv_parser* parser, char c) {
    if (peak(parser, 0) != c) {
        return false;
    } else {
        parser->pos ++;
        return true;
    }
}

static void skip_white(rv_parser* parser) {
    while (have_more(parser)) {
        if (eat(parser, '\n')) {
            parser->line ++;
        } else if (isspace(peak(parser, 0))) {
            pop(parser, 1);
        } else {
            break;
        }
    }
}

static void skip_comment(rv_parser* parser) {
    while (eat(parser, ';')) {
        while (have_more(parser) && !eat(parser, '\n')) {
            pop(parser, 1);
        }
        parser->line += 1;
    }
}

static void skip_all(rv_parser* parser) {
    while (true){
        skip_white(parser);
        if (peak(parser, 0) == ';') {
            skip_comment(parser);
        } else {
            break;
        }
    }
}

static rv_obj* parse_list(rv_parser* parser) {
    rv_obj* head;
    rv_obj* current;
    head = rv_pair_new(NULL, NULL);
    current = head;
    eat(parser, '(');
    while (have_more(parser)) {
        skip_all(parser);
        if (eat(parser, ')')) {
            return head;
        }
        rv_obj* obj = parse_expr(parser);
        // rv_pair_set_car(current, obj);
        current->pair_v.car = obj;
        assert(current->pair_v.car == obj);
        rv_obj* nil = rv_pair_new(NULL, NULL);
        // rv_pair_set_cdr(current, nil);
        current->pair_v.cdr = nil;
        current = nil;
    }
    return head;
}

static rv_obj* parse_bool(rv_parser* parser) {
    if (peak(parser, 1) == 't') {
        pop(parser, 2);
        return rv_bool_new(true);
    } else if (peak(parser, 1) == 'f') {
        pop(parser, 2);
        return rv_bool_new(false);
    } else {
        puts("parse bool error");
        exit(1);
    }
    return NULL;
}

static rv_obj* parse_number(rv_parser* parser) {
    int sign = 1;
    bool is_real = false;
    if (eat(parser, '-')) {
        sign = -1;
    } else if (eat(parser, '+')) {
        sign = 1;
    }
    size_t start_pos = parser->pos;
    while (isdigit(peak(parser, 0))) {
        pop(parser, 1);
    }
    if (eat(parser, '.')) {
        is_real = true;
    }
    while (isdigit(peak(parser, 0))) {
        pop(parser, 1);
    }
    size_t end_pos = parser->pos;
    if (start_pos == end_pos) {
        puts("Not enough number to parse");
    }
    if (is_real) {
        double v = atof(parser->src + start_pos);
        return rv_real_new(v);
    } else {
        int v = atoi(parser->src + start_pos);
        return rv_integer_new(v);
    }
}

static rv_obj* parse_symbol(rv_parser* parser) {
    size_t start_pos;
    size_t n = 1;
    bool stop = false;
    start_pos = parser->pos;
    pop(parser, 1);
    while (true) {
        if (!have_more(parser))
            break;
        if (isspace(peak(parser, 0)))
            break;
        switch (peak(parser, 0)) {
            case '\'':
            case '(':
            case ')':
            case ',':
            case '@':
                stop = true;
                break;
        }
        // FIXME: not elegant
        if (stop)
            break;
        pop(parser, 1);
        n++;
    }
    char* s = (char*)malloc(sizeof(char) * (n + 1));
    strncpy(s, (parser->src + start_pos), n);
    return rv_symbol_new(s);
}

static rv_obj* parse_string(rv_parser* parser) {
    eat(parser, '"');
    size_t start_pos;
    size_t n = 0;
    start_pos = parser->pos;
    while (true) {
        if (!have_more(parser))
            break;
        if (peak(parser, 0) == '"')
            break;
        pop(parser, 1);
        n++;
    }
    if (!eat(parser, '"')) {
        puts("Expecting '\"' to end a string.");
        exit(1);
    }
    char* s = (char*)malloc(sizeof(char) * (n + 1));
    strncpy(s, (parser->src + start_pos), n);
    return rv_string_new(s);
}

static rv_obj* parse_expr(rv_parser* parser) {
    skip_all(parser);
    if (!have_more(parser)) {
        puts("Nothing to parse.");
        exit(1);
    }
    char c = peak(parser, 0);
    switch(c) {
    case '#':
        return parse_bool(parser);
    case '(':
        return parse_list(parser);
    case '+':
    case '-':
        if (!isdigit(peak(parser, 1))) {
            break;
        }
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return parse_number(parser);
    case '"':
        return parse_string(parser);
    }
    return parse_symbol(parser);
}

rv_obj* rv_parse(rv_parser* parser) {
    return parse_expr(parser);
}
