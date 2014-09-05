#include <string.h>

#include "../src/parser.h"

int TestParseInteger() {
    rv::Parser *parser;
    rv::object::Object *obj;
    
    parser = new rv::Parser("1   ");
    obj = parser->ParseExpr();
    if (strcmp(obj->ToString(), "1") != 0) {
        return 1;
    }
    delete parser;
    delete obj;
    
    parser = new rv::Parser("    -42   ");
    obj = parser->ParseExpr();
    if (strcmp(obj->ToString(), "-42") != 0) {
        return 1;
    }
    delete parser;
    delete obj;

    return 0;
}
int TestParser() {
    int r = 0;
    r += TestParseInteger();
    return r;
}
