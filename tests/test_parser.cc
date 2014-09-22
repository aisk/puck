#include <string.h>
#include <memory>
#include <stdio.h>
#include <iostream>
#include "catch.hpp"
#include "../src/parser.h"
#include "../src/state.h"

using namespace rv;

TEST_CASE("parse integer", "[parser]") {
    InitGlobalState();
    
    char *src =  const_cast<char *>("  1");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    REQUIRE(obj->EqualTo(std::make_shared<object::Integer>(1).get()));

    src = const_cast<char *>(" -42 ");
    parser = std::make_shared<Parser>(src);
    obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    REQUIRE(obj->EqualTo(std::make_shared<object::Integer>(-42).get()));

    DestroyGlobalState();
}

TEST_CASE("parse real", "[parser]") {
    InitGlobalState();
    
    char *src = const_cast<char *>("0.618");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    REQUIRE(obj->EqualTo(std::make_shared<object::Real>(0.618).get()));

    src = const_cast<char *>("  -3.14  ");
    parser = std::make_shared<Parser>(src);
    obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    REQUIRE(obj->EqualTo(std::make_shared<object::Real>(-3.14).get()));

    DestroyGlobalState();
}

TEST_CASE("parse bool", "[parser]") {
    InitGlobalState();
    
    char *src = const_cast<char *>("   #t  ");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    REQUIRE(obj->EqualTo(std::make_shared<object::Bool>(true).get()));

    DestroyGlobalState();
}

TEST_CASE("parse symbol", "[parser]") {
    InitGlobalState();
    
    char *src = const_cast<char *>(" 'foo  ");
    char *str = const_cast<char *>("foo");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    REQUIRE(obj->EqualTo(std::make_shared<object::Symbol>(str).get()));

    DestroyGlobalState();
}

TEST_CASE("parse empty list", "[parser]") {
    InitGlobalState();
    
    auto nil = object::Pair(nullptr, nullptr);

    char *src = const_cast<char *>("   (  )   ");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    REQUIRE(obj->EqualTo(std::make_shared<object::Pair>(nullptr, nullptr).get()));

    DestroyGlobalState();
}

TEST_CASE("parse simple list", "[parser]") {
    InitGlobalState();
    
    auto one = object::Integer(1);
    auto two = object::Integer(2);
    auto tree = object::Integer(3);
    auto nil = object::Pair(nullptr, nullptr);

    char *src = const_cast<char *>(" ( 1   2 3)");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());  // TODO: leaks here
    auto expect = object::Pair(&one, std::make_shared<object::Pair>(&two, std::make_shared<object::Pair>(&tree, &nil).get()).get());
    REQUIRE(obj->EqualTo(&expect));

    DestroyGlobalState();
}

TEST_CASE("parse nested list", "[parser]") {
    InitGlobalState();
    
    auto one = object::Integer(1);
    auto two = object::Integer(2);
    auto tree = object::Integer(3);
    auto four = new object::Integer(4);
    auto five = new object::Integer(5);
    auto nil = object::Pair(nullptr, nullptr);

    char *src = const_cast<char *>(" (1 (2 3)) ");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());  // TODO: leaks here
    auto expcetInner = object::Pair(&two, std::make_shared<object::Pair>(&tree, &nil).get());
    auto expect = object::Pair(&one, std::make_shared<object::Pair>(&expcetInner, &nil).get());
    REQUIRE(obj->EqualTo(&expect));

    DestroyGlobalState();
}

TEST_CASE("parse string", "[parser]") {
    InitGlobalState();
    
    char *src = const_cast<char *>("\"hello world!\"");
    char *str = const_cast<char *>("hello world!");
    auto parser = std::make_shared<Parser>(src);
    auto obj = std::shared_ptr<object::Object>(parser->ParseExpr());
    REQUIRE(obj->EqualTo(std::make_shared<object::String>(str).get()));

    DestroyGlobalState();
}
