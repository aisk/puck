#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_object();
int test_parser();

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Useage: testing [testcase]\n");
        exit(2);
    }
    char *testcase = argv[1];
    if (strcmp("object", testcase) == 0) {
        return test_object();
    } else if (strcmp("parser", testcase) == 0) {
        return test_parser();
    }
    return 3;
}
