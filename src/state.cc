#include <stdlib.h>
#include <iostream>
#include "state.h"

using namespace rv;

rv::State *rv::gstate;

void rv::InitGlobalState() {
    rv::gstate = new rv::State();
}

void rv::DestroyGlobalState() {
    auto objs = rv::gstate->GetObjs();
    for (auto iter = objs.begin(); iter != objs.end(); ++iter) {
        auto obj = *iter;
        delete obj;
    }
    delete rv::gstate;
}

