#include <stdio.h>

extern int test_parse_number();
extern int test_parse_bool();
extern int test_parse_symbol();
extern int test_parse_string();
extern int test_parse_list();

extern int test_dict_new();
extern int test_dict_set();
extern int test_dict_get();
extern int test_dict_update();

extern int test_env_new();

extern int test_obj_is_nil();

#define TEST(fn) fn();printf("    \e[92m✓ \e[90m%s\e[0m\n", #fn);

int main() {
    TEST(test_parse_bool);
    TEST(test_parse_number);
    TEST(test_parse_symbol);
    TEST(test_parse_string);
    TEST(test_parse_list);
    TEST(test_dict_new);
    TEST(test_dict_set);
    TEST(test_dict_get);
    TEST(test_dict_update);
    TEST(test_env_new);
    TEST(test_obj_is_nil);
    return 0;
}
