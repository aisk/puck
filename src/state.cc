#include <stdlib.h>
#include "state.h"


rv::State *rv::gstate;

void rv::InitGlobalState() {
    rv::gstate = new rv::State();
}

void rv::DestroyGlobalState() {
    delete rv::gstate;
}
