#define CATCH_CONFIG_RUNNER

#include <catch2/catch.hpp>
#include "../src/state.h"
#include "test_object.cc"
#include "test_parser.cc"
#include "test_env.cc"

int main(int argc, char* argv[]) {
    rv_state* state = rv_state_new();
    rv_state_set_current(state);
    int result = Catch::Session().run(argc, argv);
    rv_state_free(state);
    return result;
}
