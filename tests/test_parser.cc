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

int TestParseSymbol() {
    char *src = const_cast<char *>(" 'foo  ");
    char *str = const_cast<char *>("foo");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    if (!obj->EqualTo(new object::Symbol(str))) {
        return 1;
    }

    return 0;
}

int TestParseList() {
    auto one = new object::Integer(1);
    auto two = new object::Integer(2);
    auto tree = new object::Integer(3);
    auto four = new object::Integer(4);
    auto five = new object::Integer(5);
    auto nil = new object::Pair(nullptr, nullptr);

    char *src = const_cast<char *>("   (  )   ");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    if (! obj->EqualTo(new object::Pair(nullptr, nullptr))) {
        return 1;
    }

    src = const_cast<char *>(" ( 1   2 3)");
    parser = std::make_shared<Parser>(src);
    obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    auto expect = new object::Pair(one, new object::Pair(two, new object::Pair(tree, nil)));
    if (! obj->EqualTo(expect)) {
        return 1;
    }

    src = const_cast<char *>(" (1 (2 3)) ");
    parser = std::make_shared<Parser>(src);
    obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    auto expcetInner = new object::Pair(two, new object::Pair(tree, nil));
    expect = new object::Pair(one, new object::Pair(expcetInner, nil));
    if (! obj->EqualTo(expect)) {
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
    r += TestParseSymbol();
    r += TestParseList();
    r += TestParseString();
    return r;
}

