#include <stdio.h>
#include "../src/object.h"

int TestObject() {
    auto integer = new rv::object::Integer(1);
    puts(integer->ToString());
    integer->GetType();
    return 0;
}
