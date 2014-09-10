#include <stdio.h>
#include "../src/object.h"

int TestObject() {
    return 0;
}

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
