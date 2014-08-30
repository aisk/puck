#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "dict.h"

static int rv_dict_node_comp(rv_dict_node* node_a, rv_dict_node* node_b) {
    int ret = strcmp(node_a->key->string_v, node_b->key->string_v);
    // printf("%s %s %d\n", node_a->key->string_v, node_b->key->string_v, ret);
    return ret;
}

RB_HEAD(rv_dict, rv_dict_node);
RB_GENERATE_STATIC(rv_dict, rv_dict_node, entry, rv_dict_node_comp);

rv_dict* rv_dict_new() {
    rv_dict* head = (rv_dict*)malloc(sizeof(rv_dict));
    RB_INIT(head);

    // rv_dict_node* node;
    // for (node = RB_MIN(rv_dict, head); node != NULL; node = RB_NEXT(rv_dict, &rv_dict_head, node)) {
    //     // printf("%p\n", node);
    //     // rv_obj_print(node->obj);
    // }
    return head;
}

void rv_dict_set(rv_dict* dict, rv_obj* key, rv_obj* value) {
    rv_dict_node match_node = {.key=key};
    rv_dict_node* node = RB_FIND(rv_dict, dict, &match_node);
    if (node == NULL) {
        rv_dict_node *node = (rv_dict_node*)malloc(sizeof(rv_dict_node));
        node->key = key;
        node->value = value;
        RB_INSERT(rv_dict, dict, node);
    } else {
        node->value = value;
    }
}

rv_obj* rv_dict_get(rv_dict* dict, rv_obj* key) {
    rv_dict_node match_node = {.key=key};
    rv_dict_node* node = RB_FIND(rv_dict, dict, &match_node);
    if (node == NULL) {
        return NULL;
    }
    return node->value;
}

void rv_dict_print(rv_dict* dict) {
    rv_dict_node* node;
    RB_FOREACH(node, rv_dict, dict) {
        printf("%s: ", node->key->string_v);
        rv_obj_print(node->value);
        printf("\n");
    }
}
