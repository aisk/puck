#include <stdio.h>
#include <string.h>
#include "../src/object.h"

using namespace rv;

int TestInteger() {
    auto one = new rv::object::Integer(1);
    auto two = new rv::object::Integer(2);
    if (one->GetType() != rv::object::type::INTEGER) {
        return 1;
    }
    if (one->EqualTo(two)) {
        return 1;
    }
    if (!one->EqualTo( new rv::object::Integer(1) )) {
        return 1;
    }
    return 0;
}

int TestReal() {
    object::Object *obj = new object::Real(0.618);
    if (obj->GetType() != object::type::REAL) {
        return 1;
    }
    return 0;
}

int TestBool() {
    object::Object *True = new object::Bool(true);
    if (True->GetType() != object::type::BOOL) {
        return 1;
    }
    if (strcmp(True->ToString(), "#t") != 0) {
        return 1;
    }
    return 0;
}

int TestString() {
    char *str = const_cast<char *>("hello world!");
    object::Object *obj = new object::String(str);
    if (obj->GetType() != object::type::STRING) {
        return 1;
    }
    if (strcmp(obj->ToString(), "\"hello world!\"") != 0) {
        return 1;
    }
    return 0;
}

int TestObject() {
    int r = 0;
    r += TestInteger();
    r += TestReal();
    r += TestBool();
    r += TestString();
    return r;
}
