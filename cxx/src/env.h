#ifndef PUCK_ENV_H
#define PUCK_ENV_H

#include <map>
#include "dict.h"

namespace puck {
class Env {
    std::map<object::Object *, object::Object *> store;
    Env *outer;
public:
    Env();
    Env(Env &);
    inline Env *GetOuter() { return outer; };
    object::Object *Get(object::Object &key);
    void Set(object::Object &key, object::Object &value);
};

} // namespace puck

struct rv_env;
typedef struct rv_env rv_env;

struct rv_env {
    rv_dict* dict;
    rv_env* outer;
};

rv_env* rv_env_new(rv_env* outer);
void rv_env_set(rv_env* env, rv_obj* key, rv_obj* value);
rv_obj* rv_env_get(rv_env* env, rv_obj* key);

#endif
