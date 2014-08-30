#ifndef RIVAI_ENV_H
#define RIVAI_ENV_H

#include "dict.h"

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
