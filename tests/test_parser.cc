#include <string.h>
#include <memory>
#include <stdio.h>
#include <iostream>

#include "../src/parser.h"

using namespace rv;

int TestParseInteger() {

    char *src =  const_cast<char *>("  1");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    if (!obj->EqualTo(new object::Integer(1))) {
        return 1;
    }
    
    src = const_cast<char *>(" -42 ");
    parser = std::make_shared<Parser>(src);
    obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    if (!obj->EqualTo(new object::Integer(-42))) {
        return 1;
    }

    return 0;
}

int TestParseReal() {
    char *src = const_cast<char *>("0.618");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    if (!obj->EqualTo(new object::Real(0.618))) {
        return 1;
    }

    src = const_cast<char *>("  -3.14  ");
    parser = std::make_shared<Parser>(src);
    obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    if (!obj->EqualTo(new object::Real(-3.14))) {
        return 1;
    }
    
    return 0;
}

int TestParseBool() {
    char *src = const_cast<char *>("   #t  ");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    if (!obj->EqualTo(new object::Bool(true))) {
        return 1;
    }

    return 0;
}

int TestParseString() {
    char *src = const_cast<char *>(" \"hello world!\"  ");
    char *str = const_cast<char *>("hello world!");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    if (!obj->EqualTo(new object::String(str))) {
        return 1;
    }

    return 0;
}

int TestParser() {
    int r = 0;
    r += TestParseInteger();
    r += TestParseReal();
    r += TestParseBool();
    r += TestParseString();
    return r;
}
