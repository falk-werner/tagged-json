cmake_minimum_required(VERSION 4.0)
project(tagged-json)

option(WITHOUT_TEST "Disable unit test" OFF)

set(CMAKE_CXX_STANDARD 17)

add_library(tagged-json STATIC
    lib/tagged-json/tokenizer.cpp
)
target_include_directories(tagged-json PUBLIC inc)
target_include_directories(tagged-json PRIVATE lib)

if(NOT(WITHOUT_TEST))

enable_testing()
include(CTest)

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(alltests
    test-src/test_tokenizer.cpp
    test-src/test_tokenize_comments.cpp
    test-src/test_tokenize_keywords.cpp
    test-src/test_tokenize_uint.cpp
    test-src/test_tokenize_sint.cpp
    test-src/test_tokenize_f64.cpp
    test-src/test_tokenize_string.cpp
    test-src/test_tokenize_tag.cpp
)

target_link_libraries(alltests PRIVATE tagged-json GTest::gtest GTest::gtest_main)
target_include_directories(alltests PRIVATE lib)
gtest_discover_tests(alltests)

endif()
