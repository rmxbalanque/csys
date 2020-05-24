
# Configure CMAKE Version.
cmake_minimum_required(VERSION 3.1...3.16)

# Add Custom In-Game CLI Project.
project(ccli VERSION 0.1
             LANGUAGES CXX)

# Setup module subdirectory.
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Include tests.
set(CCLI_INCLUDE_EXAMPLES 1 CACHE STRING "Include and build CCLI examples targets")

## TEMPORARY #####################

# Add core ccli target.
add_library(ccli src/ccli.cpp)
target_include_directories(ccli PUBLIC include/ccli)

##################################

# Examples.
add_subdirectory(examples)