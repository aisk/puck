#include <assert.h>
#include <string.h>
#include "../src/parser.h"
#include "../src/object.h"

extern void expect(rv_obj* obj, rv_type type, void* value);
extern void expect_nil(rv_obj* obj);

int test_parse_number() {
    rv_parser* parser;
    rv_obj* obj;

    parser = rv_parser_new("1      ");
    obj = rv_parse(parser);
    int v1 = 1;
    expect(obj, RV_TYPE_INTEGER, &v1);

    parser = rv_parser_new("   42      ");
    obj = rv_parse(parser);
    int v2 = 42;
    expect(obj, RV_TYPE_INTEGER, &v2);

    parser = rv_parser_new("   1.234      ");
    obj = rv_parse(parser);
    double v3 = 1.234;
    expect(obj, RV_TYPE_REAL, &v3);

    return 0;
}

int test_parse_bool() {
    rv_parser* parser;
    rv_obj* obj;

    parser = rv_parser_new("  #t ");
    obj = rv_parse(parser);
    bool v_true = true;
    expect(obj, RV_TYPE_BOOL, &v_true);

    parser = rv_parser_new("   #f  ");
    obj = rv_parse(parser);
    bool v_false = false;
    expect(obj, RV_TYPE_BOOL, &v_false);

    return 0;
}

int test_parse_symbol() {
    rv_parser* parser;
    rv_obj* obj;

    parser = rv_parser_new("add ");
    obj = rv_parse(parser);
    expect(obj, RV_TYPE_SYMBOL, "add");

    parser = rv_parser_new("   add42 ");
    obj = rv_parse(parser);
    expect(obj, RV_TYPE_SYMBOL, "add42");

    parser = rv_parser_new("   do or die ");
    obj = rv_parse(parser);
    expect(obj, RV_TYPE_SYMBOL, "do");

    return 0;
}

int test_parse_string() {
    rv_parser* parser;
    rv_obj* obj;

    parser = rv_parser_new("\"Hi!\"");
    obj = rv_parse(parser);
    expect(obj, RV_TYPE_STRING, "Hi!");

    return 0;
}

int test_parse_list() {
    rv_parser* parser;
    rv_obj* obj;
    
    parser = rv_parser_new("(  )");
    obj = rv_parse(parser);

    parser = rv_parser_new("( 42 )");
    obj = rv_parse(parser);
    int v42 = 42;
    expect(obj->pair_v.car, RV_TYPE_INTEGER, &v42);
    expect_nil(obj->pair_v.cdr);

    parser = rv_parser_new("( #t 42 )");
    obj = rv_parse(parser);
    bool vtrue = true;
    expect(obj->pair_v.car, RV_TYPE_BOOL, &vtrue);
    assert(obj->pair_v.cdr->t == RV_TYPE_PAIR);
    expect(obj->pair_v.cdr->pair_v.car, RV_TYPE_INTEGER, &v42);
    expect_nil(obj->pair_v.cdr->pair_v.cdr);

    return 0;
}
