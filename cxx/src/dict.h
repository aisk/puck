#ifndef RIVAI_DICT_H
#define RIVAI_DICT_H

#include <uv-private/tree.h>
#include "object.h"

struct rv_dict_node;
typedef struct rv_dict_node rv_dict_node;
typedef struct rv_dict rv_dict;

struct rv_dict_node {
    RB_ENTRY(rv_dict_node) entry;
    rv_obj* key;
    rv_obj* value;
};

rv_dict* rv_dict_new();
void rv_dict_set(rv_dict* dict, rv_obj* key, rv_obj* value);
rv_obj* rv_dict_get(rv_dict* dict, rv_obj* key);
void rv_dict_print(rv_dict* dict);

#endif
