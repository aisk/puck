#include <string.h>

#include "../src/parser.h"

int TestParseNumber() {
    auto parser = new rv::Parser("1   ");
    auto r = parser->ParseExpr();
    if (strcmp(r->ToString(), "1") != 0) {
        return 1;
    }
    delete parser;
    return 0;
}
int TestParser() {
    int r = 0;
    r += TestParseNumber();
    return r;
}
