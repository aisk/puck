#ifndef PUCK_STATE_H
#define PUCK_STATE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct rv_obj;
typedef struct rv_obj rv_obj;

struct rv_state;
typedef struct rv_state rv_state;

rv_state* rv_state_new(void);
void rv_state_set_current(rv_state* state);
rv_state* rv_state_current(void);
void rv_state_track(rv_obj* obj);
void rv_state_untrack(rv_obj* obj);
size_t rv_state_object_count(const rv_state* state);
void rv_state_free(rv_state* state);

#ifdef __cplusplus
}
#endif

#endif
