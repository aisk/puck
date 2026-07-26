cmake_minimum_required(VERSION 3.16)
project(puck LANGUAGES C CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")

add_library(puck_core STATIC ${SOURCES})
target_include_directories(puck_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_options(puck_core PRIVATE -Wall -Wextra -Wpedantic -Wstrict-prototypes)

add_executable(puck "src/main.c")
target_compile_options(puck PRIVATE -Wall -Wextra)
target_link_libraries(puck PRIVATE puck_core)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.10
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

add_executable(run_test
    "tests/main.cc"
    "tests/test_env.cc"
    "tests/test_eval.cc"
    "tests/test_object.cc"
    "tests/test_parser.cc"
)
target_compile_options(run_test PRIVATE -Wall -Wextra)
target_link_libraries(run_test PRIVATE puck_core Catch2::Catch2)

enable_testing()
add_test(NAME test COMMAND run_test)
