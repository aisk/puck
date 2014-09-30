#ifndef PUCK_STATE_H
#define PUCK_STATE_H

#include <list>
#include "object.h"

namespace puck {

class State {
    std::list<object::Object *> objs;
public:
    inline std::list<object::Object *> GetObjs() { return objs; }
    template<typename T> T *allocate() {
        T* a = new T();
        objs.push_back(a);
        return a;
    }
    template<typename T> T *allocate(double v) {
        T* a = new T(v);
        objs.push_back(a);
        return a;
    }
    template<typename T> T *allocate(char *v) {
        T* a = new T(v);
        objs.push_back(a);
        return a;
    }
    template<typename T> T *allocate(object::Object *v1, object::Object *v2) {
        T* a = new T(v1, v2);
        objs.push_back(a);
        return a;
    }
};

extern State *gstate;

void InitGlobalState();
void DestroyGlobalState();

} // namespace puck

#endif
