#ifndef RV_STATE_H
#define RV_STATE_H

#include <list>
#include "object.h"

namespace rv {

class State {
    std::list<rv::object::Object *> objs;
public:
    inline std::list<rv::object::Object *> GetObjs() { return objs; }
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

} // namespace rv

#endif
