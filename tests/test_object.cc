#include <stdio.h>
#include "../src/object.h"

int test_object() {
    auto integer = new rv::object::Integer(1);
    integer->ToString();
    integer->GetType();
    return 0;
}
