#include <string.h>
#include <memory>
#include <stdio.h>

#include "../src/parser.h"

using namespace rv;

int TestParseInteger() {
    auto parser = std::make_shared<rv::Parser>("1   ");
    auto obj = std::shared_ptr<rv::object::Object>(parser->ParseExpr());
    if (!obj->EqualTo(new object::Integer(1))) {
        return 1;
    }
    
    parser = std::make_shared<rv::Parser>("  -42   ");
    obj = std::shared_ptr<rv::object::Object>(parser->ParseExpr());
    if (!obj->EqualTo(new object::Integer(-42))) {
        return 1;
    }

    return 0;
}

int TestParseReal() {
    auto parser = std::make_shared<rv::Parser>("0.618");
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    if (!obj->EqualTo(new object::Real(0.618))) {
        return 1;
    }
    
    return 0;
}

int TestParser() {
    int r = 0;
    r += TestParseReal();
    r += TestParseInteger();
    return r;
}
