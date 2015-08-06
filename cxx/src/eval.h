#ifndef RIVAI_EVAL_H
#define RIVAI_EVAL_H

#include "env.h"

using namespace puck;

namespace puck {

object::Object *Eval(object::Object &, Env &);

} // namespace puck

rv_obj* rv_eval(rv_obj* obj, rv_env* env);

#endif
