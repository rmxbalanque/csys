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

#if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    target_compile_features(ccli PRIVATE cxx_std_17)
#endif()

# -----------------------------------------------------------------------------
# Build Settings
# -----------------------------------------------------------------------------

# Include tests.
option(CCLI_BUILD_EXAMPLES "Include and build CCLI examples targets" ON)

# Build shared option
option(CCLI_BUILD_SHARED "Build shared library" OFF)

# Precompiled headers option
option(CCLI_ENABLE_PCH "Build library using precompiled headers to improve compilation times" ON)

# Testing options.
option(CCLI_BUILD_TESTS "Build tests" ON)

# -----------------------------------------------------------------------------
# Build Static/Shared Library
# -----------------------------------------------------------------------------

# ccli files
set(CCLI_SOURCES src/ccli.cpp)
set(CCLI_HEADERS include/ccli/ccli.h include/ccli/ccli_arguments.h include/ccli/ccli_command.h)

# Add core ccli target.
if(CCLI_BUILD_SHARED)
    add_library(ccli SHARED ${CCLI_SOURCES} ${CCLI_HEADERS})
else()
    add_library(ccli STATIC ${CCLI_SOURCES} ${CCLI_HEADERS})
    target_compile_definitions(ccli PRIVATE CCLI_BUILD_STATIC)  # Used for export macros.
endif()

# Define ccli namespace
add_library(ccli::ccli ALIAS ccli)

# Library public include directories
target_include_directories(ccli PUBLIC include/ccli)

# Library private include directories
#target_include_directories(ccli PRIVATE )

# Set precompiled headers if supported and enabled
if(COMMAND target_precompile_headers AND CCLI_ENABLE_PCH)
    target_precompile_headers(ccli PRIVATE include/ccli/ccli_pch.h)
endif()

# Export no symbols by default. (To ensure cross-compiler consistent behaviour)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Generate export header automatically
include(GenerateExportHeader)
GENERATE_EXPORT_HEADER(ccli
                        BASE_NAME ccli
                        EXPORT_MACRO_NAME CCLI_API
                        EXPORT_FILE_NAME ${ccli_SOURCE_DIR}/include/ccli/ccli_api.h
                        STATIC_DEFINE CCLI_BUILD_STATIC)
# -----------------------------------------------------------------------------
# Build binaries
# -----------------------------------------------------------------------------

if(CCLI_BUILD_EXAMPLES)
    message(STATUS "Generating Example(s)")
    add_subdirectory(examples)
endif()

if(CCLI_BUILD_TESTS)
    message(STATUS "Generating tests")
    add_subdirectory(tests)
endif()

# TODO: Add IDE support for headers. (source_group)
# TODO: CMake Install support.
# TODO: Look into header only version support of ccli.
# TODO: Add clang-tidy, cpplint, include-what-you-use, and link-what-you-use.
# TODO: Add Debug compiler flags.

# For error ignoring. (Should be removed later).
set(CMAKE_C_COMPILER ${CMAKE_C_COMPILER})