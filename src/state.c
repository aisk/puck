#include <stdlib.h>

#include "object.h"
#include "state.h"

typedef struct rv_state_node {
    rv_obj* obj;
    struct rv_state_node* next;
} rv_state_node;

struct rv_state {
    rv_state_node* objects;
    size_t object_count;
};

static rv_state* current_state = NULL;

rv_state* rv_state_new(void) {
    rv_state* state = (rv_state*)calloc(1, sizeof(rv_state));
    return state;
}

void rv_state_set_current(rv_state* state) {
    current_state = state;
}

rv_state* rv_state_current(void) {
    return current_state;
}

void rv_state_track(rv_obj* obj) {
    if (current_state == NULL || obj == NULL) {
        return;
    }
    rv_state_node* node = (rv_state_node*)malloc(sizeof(rv_state_node));
    node->obj = obj;
    node->next = current_state->objects;
    current_state->objects = node;
    current_state->object_count++;
}

void rv_state_untrack(rv_obj* obj) {
    if (current_state == NULL) {
        return;
    }
    rv_state_node** link = &current_state->objects;
    while (*link != NULL) {
        if ((*link)->obj == obj) {
            rv_state_node* removed = *link;
            *link = removed->next;
            free(removed);
            current_state->object_count--;
            return;
        }
        link = &(*link)->next;
    }
}

size_t rv_state_object_count(const rv_state* state) {
    return state == NULL ? 0 : state->object_count;
}

void rv_state_free(rv_state* state) {
    if (state == NULL) {
        return;
    }
    rv_state_node* node = state->objects;
    while (node != NULL) {
        rv_state_node* next = node->next;
        if (node->obj->t == RV_TYPE_SYMBOL || node->obj->t == RV_TYPE_STRING) {
            free(node->obj->string_v);
        }
        free(node->obj);
        free(node);
        node = next;
    }
    if (current_state == state) {
        current_state = NULL;
    }
    free(state);
}
