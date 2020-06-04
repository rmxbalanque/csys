# Configure CMAKE Version.
cmake_minimum_required(VERSION 3.1...3.16)

# Start test project.
project(ccli_tests LANGUAGES CXX)

# Stand alone build (TODO: Read more about this.
if(NOT TARGET ccli)
    find_package(ccli REQUIRED)
endif()

# Utilities.
include(../cmake/ccli_utils.cmake)

# Sources.
set(CCLI_TEST_SOURCES
        test_autocomplete.cpp
        test_system.cpp
        main.cpp)

# Set up testing.
enable_testing()

# Helper func.
function(ccli_prepare_test test_target ccli_lib)
    add_executable(${test_target} ${CCLI_TEST_SOURCES})
    ccli_enable_warnings(${test_target})
    target_link_libraries(${test_target} PRIVATE ${ccli_lib})
    add_test(NAME ${test_target} COMMAND ${test_target})
    #set_tests_properties(${test_target} PROPERTIES RUN_SERIAL ON)
endfunction()

# Build tests with ccli library.
if (CCLI_BUILD_TESTS)
    ccli_prepare_test(ccli_tests ccli::ccli)
endif()