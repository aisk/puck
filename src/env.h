#ifndef PUCK_ENV_H
#define PUCK_ENV_H

#include "dict.h"

#ifdef __cplusplus
extern "C" {
#endif

struct rv_env;
typedef struct rv_env rv_env;

struct rv_env {
    rv_dict* dict;
    rv_env* outer;
};

rv_env* rv_env_new(rv_env* outer);
void rv_env_free(rv_env* env);
void rv_env_set(rv_env* env, rv_obj* key, rv_obj* value);
rv_obj* rv_env_get(rv_env* env, rv_obj* key);

#ifdef __cplusplus
}
#endif

#endif
