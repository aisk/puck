CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(puck)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall")
FILE(GLOB SOURCES "./src/[^main]*.cc")
ADD_EXECUTABLE(puck ${SOURCES} "./src/main.cc")

ADD_EXECUTABLE(run_test "./tests/main.cc" ${SOURCES})

set(CATCH_PATH "${CMAKE_SOURCE_DIR}/tests/catch.hpp")
set(CATCH_URL "https://raw.github.com/philsquared/Catch/master/single_include/catch.hpp")

if (NOT EXISTS "${CATCH_PATH}")
    file(DOWNLOAD "${CATCH_URL}" "${CATCH_PATH}")
endif()

ENABLE_TESTING()
ADD_TEST(test run_test)
