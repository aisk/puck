cmake_minimum_required(VERSION 3.16)
project(puck LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc")

add_executable(puck ${SOURCES} "src/main.cc")
target_compile_options(puck PRIVATE -Wall -Wextra)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.10
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

add_executable(run_test "tests/main.cc" ${SOURCES})
target_compile_options(run_test PRIVATE -Wall -Wextra)
target_link_libraries(run_test PRIVATE Catch2::Catch2)

enable_testing()
add_test(NAME test COMMAND run_test)
