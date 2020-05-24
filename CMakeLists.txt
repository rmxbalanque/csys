# Configure CMAKE Version.
cmake_minimum_required(VERSION 3.1...3.16)

# -----------------------------------------------------------------------------
# Start CCLI project.
# -----------------------------------------------------------------------------

# Add Custom In-Game CLI Project.
project(ccli VERSION 0.1 LANGUAGES CXX)
message(STATUS "Build CCLI: ${PROJECT_VERSION}")

# Setup module subdirectory.
set(CMAKE_MODULE_PATH  "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# -----------------------------------------------------------------------------
# Set default build to debug
# -----------------------------------------------------------------------------

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose Release or Debug" FORCE)
endif()

# -----------------------------------------------------------------------------
# Compiler config
# -----------------------------------------------------------------------------

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# -----------------------------------------------------------------------------
# Build Settings
# -----------------------------------------------------------------------------

# Include tests.
option(CCLI_INCLUDE_EXAMPLES "Include and build CCLI examples targets" ON)

# Build shared option
option(CCLI_BUILD_SHARED "Build shared library" OFF)

# Precompiled headers option
option(CCLI_ENABLE_PCH "Build library using precompiled headers to improve compilation times" OFF)

# Testing options.
option(CCLI_BUILD_TESTS "Build tests" OFF)

# -----------------------------------------------------------------------------
# Build Static/Shared Library
# -----------------------------------------------------------------------------

# ccli files
set(CCLI_SOURCES src/ccli.cpp)
set(CCLI_HEADERS include/ccli/ccli.h)

# Add core ccli target.
if(CCLI_BUILD_SHARED)
    add_library(ccli SHARED ${CCLI_SOURCES} ${CCLI_HEADERS})
else()
    add_library(ccli STATIC ${CCLI_SOURCES} ${CCLI_HEADERS})
endif()

# Define ccli namespace
add_library(ccli::ccli ALIAS ccli)

# Library public include directories
target_include_directories(ccli PUBLIC include/ccli)

# Set precompiled headers if supported and enabled
if(COMMAND target_precompile_headers AND CCLI_ENABLE_PCH)
    target_precompile_headers(cclie PRIVATE src/ccli_pch.h)
endif()

# -----------------------------------------------------------------------------
# Build binaries
# -----------------------------------------------------------------------------

if(CCLI_INCLUDE_EXAMPLES)
    add_subdirectory(examples)
endif()

# TODO: Add IDE support for headers.
# TODO: CMake Install support.
# TODO: Look into header only version support of ccli.
# TODO: Add Shared Library Support for Windows.