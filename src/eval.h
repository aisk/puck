#ifndef RIVAI_EVAL_H
#define RIVAI_EVAL_H

#include "env.h"

rv_obj* rv_eval(rv_obj* obj, rv_env* env);
void rv_env_add_builtins(rv_env* env);

#endif
