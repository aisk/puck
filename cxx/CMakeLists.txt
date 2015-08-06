cmake_minimum_required(VERSION 2.8)
project(puck)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall")
file(GLOB SOURCES "./src/[^main]*.cc")
add_executable(puck ${SOURCES} "./src/main.cc")

add_executable(run_test "./tests/main.cc" ${SOURCES})

set(CATCH_PATH "${CMAKE_SOURCE_DIR}/tests/catch.hpp")
set(CATCH_URL "https://raw.github.com/philsquared/Catch/master/single_include/catch.hpp")

if (NOT EXISTS "${CATCH_PATH}")
    file(DOWNLOAD "${CATCH_URL}" "${CATCH_PATH}")
endif()

enable_testing()
add_test(test run_test)
