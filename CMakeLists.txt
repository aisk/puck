CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(rivai)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
FILE(GLOB SOURCES "./src/[^main]*.cc")
ADD_EXECUTABLE(rivai ${SOURCES} "./src/main.cc")

FILE(GLOB TESTING_SOURCES "./tests/*.cc")
ADD_EXECUTABLE(run_test ${TESTING_SOURCES} ${SOURCES})

ENABLE_TESTING()
ADD_TEST(test_object run_test object)
ADD_TEST(test_parser run_test parser)
