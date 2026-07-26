#ifndef RIVAI_EVAL_H
#define RIVAI_EVAL_H

#include "env.h"

#ifdef __cplusplus
extern "C" {
#endif

rv_obj* rv_eval(rv_obj* obj, rv_env* env);
void rv_env_add_builtins(rv_env* env);

#ifdef __cplusplus
}
#endif

#endif
