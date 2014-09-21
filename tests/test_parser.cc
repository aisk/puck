#include <string.h>
#include <memory>
#include <stdio.h>
#include <iostream>
#include "catch.hpp"
#include "../src/parser.h"

using namespace rv;

TEST_CASE("parse integer", "[parser]") {
    char *src =  const_cast<char *>("  1");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    REQUIRE(obj->EqualTo(std::make_shared<object::Integer>(1).get()));

    src = const_cast<char *>(" -42 ");
    parser = std::make_shared<Parser>(src);
    obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    REQUIRE(obj->EqualTo(std::make_shared<object::Integer>(-42).get()));
}

TEST_CASE("parse real", "[parser]") {
    char *src = const_cast<char *>("0.618");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    REQUIRE(obj->EqualTo(std::make_shared<object::Real>(0.618).get()));

    src = const_cast<char *>("  -3.14  ");
    parser = std::make_shared<Parser>(src);
    obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    REQUIRE(obj->EqualTo(std::make_shared<object::Real>(-3.14).get()));
}

TEST_CASE("parse bool", "[parser]") {
    char *src = const_cast<char *>("   #t  ");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    REQUIRE(obj->EqualTo(std::make_shared<object::Bool>(true).get()));
}

TEST_CASE("parse symbol", "[parser]") {
    char *src = const_cast<char *>(" 'foo  ");
    char *str = const_cast<char *>("foo");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    REQUIRE(obj->EqualTo(std::make_shared<object::Symbol>(str).get()));
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

// TEST_CASE("parse string", "[parser]") {
//     char *src = const_cast<char *>(" \"hello world!\"  ");
//     char *str = const_cast<char *>("hello world!");
//     auto parser = std::make_shared<Parser>(src);
//     auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
//     REQUIRE(obj->EqualTo(std::make_shared<object::String>(str).get()));
// }
