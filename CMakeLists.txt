CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(rivai)

FILE(GLOB SOURCES "./src/[^main]*.cc")
ADD_EXECUTABLE(rivai ${SOURCES} "./src/main.cc")

FILE(GLOB TESTING_SOURCES "./tests/*.cc")
ADD_EXECUTABLE(testing ${TESTING_SOURCES} ${SOURCES})

ENABLE_TESTING()
ADD_TEST(test_object testing object)
ADD_TEST(test_parser testing parser)
