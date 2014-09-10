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
    // char *src = const_cast<char *>("0.618");
    char *src = new char[6]{'0', '.', '6', '1', '8', 0};
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    std::cout << obj->ToString() << std::endl;
    if (!obj->EqualTo(new object::Real(0.618))) {
        return 1;
    }
    
    return 0;
}

int TestParser() {
    int r = 0;
    r += TestParseInteger();
    r += TestParseReal();
    return r;
}
