#include <stdlib.h>
#include <iostream>
#include "state.h"

using namespace puck;

puck::State *puck::gstate;

void puck::InitGlobalState() {
    puck::gstate = new puck::State();
}

void puck::DestroyGlobalState() {
    auto objs = puck::gstate->GetObjs();
    for (auto iter = objs.begin(); iter != objs.end(); ++iter) {
        auto obj = *iter;
        delete obj;
    }
    delete puck::gstate;
}

