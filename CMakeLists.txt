# CMake Project configuration was modeled after: https://github.com/gabime/spdlog

# Configure CMAKE Version.
cmake_minimum_required(VERSION 3.1...3.16)

# -----------------------------------------------------------------------------
# Start CCLI project.
# -----------------------------------------------------------------------------
include(cmake/ccli_utils.cmake)

# Add Custom In-Game CLI Project.
project(ccli VERSION 0.0.1 LANGUAGES CXX)
message(STATUS "Build CCLI: ${PROJECT_VERSION}")

# Setup module subdirectory.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

# -----------------------------------------------------------------------------
# Set default build to debug
# -----------------------------------------------------------------------------

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose Release or Debug" FORCE)
endif ()

# -----------------------------------------------------------------------------
# Compiler config
# -----------------------------------------------------------------------------

if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif ()

# -----------------------------------------------------------------------------
# Build Settings
# -----------------------------------------------------------------------------

# Include tests.
option(CCLI_BUILD_EXAMPLES "Include and build CCLI examples targets" OFF) # ON

# Build shared option
option(CCLI_BUILD_SHARED "Build shared library" OFF) # ON

# Precompiled headers option
option(CCLI_ENABLE_PCH "Build library using precompiled headers to improve compilation times" OFF) # ON

# Testing options.
option(CCLI_BUILD_TESTS "Build tests" OFF) # ON

# CCLI compiler warnings
option(CCLI_BUILD_WARNINGS "Enable compiler warnings" OFF) # ON

# - Tools -

# Include-What-You-Use
option(CCLI_ENABLE_IWYU "Enables the usage of include what you use" OFF) # ON
set(CCLI_IWYU_CLANG_VERSION "9" CACHE STRING "Clang version that include-what-you-use will use") # 9

# -----------------------------------------------------------------------------
# Build Static/Shared Library
# -----------------------------------------------------------------------------

# ccli files
set(CCLI_SOURCES src/ccli.cpp)
set(CCLI_HEADERS include/ccli/ccli.h include/ccli/ccli_autocomplete.h include/ccli/ccli_arguments.h include/ccli/ccli_command.h include/ccli/ccli_string.h)

# Add core ccli target.
if (CCLI_BUILD_SHARED)
    add_library(ccli SHARED ${CCLI_SOURCES} ${CCLI_HEADERS})
    target_compile_definitions(ccli PUBLIC CCLI_SHARED_LIB)
else ()
    add_library(ccli STATIC ${CCLI_SOURCES} ${CCLI_HEADERS})
    target_compile_definitions(ccli PUBLIC CCLI_STATIC_LIB)  # Used for export macros.
endif ()

# Enable warnings.
ccli_enable_warnings(ccli)

# Define ccli namespace
add_library(ccli::ccli ALIAS ccli)

# By default we are compiling this library.
target_compile_definitions(ccli PUBLIC CCLI_COMPILED_LIB)
#target_compile_definitions(ccli PUBLIC CCLI_HEADER_ONLY)

# Library public include directories
target_include_directories(ccli PUBLIC include/ccli/)

# Library private include directories
#target_include_directories(ccli PRIVATE )

# Set precompiled headers if supported and enabled
if (COMMAND target_precompile_headers AND CCLI_ENABLE_PCH)
    target_precompile_headers(ccli PRIVATE include/ccli/ccli_pch.h)
endif ()

# Export no symbols by default. (To ensure cross-compiler consistent behaviour)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Generate export header automatically
#include(GenerateExportHeader)
#GENERATE_EXPORT_HEADER(ccli
#        BASE_NAME ccli
#        EXPORT_MACRO_NAME CCLI_API
#        EXPORT_FILE_NAME ${ccli_SOURCE_DIR}/include/ccli/ccli_api.h
#        STATIC_DEFINE CCLI_BUILD_STATIC)

# -----------------------------------------------------------------------------
# Header only version
# -----------------------------------------------------------------------------

# Add header version ccli
add_library(ccli_header_only INTERFACE)
add_library(ccli::ccli_header_only ALIAS ccli_header_only)

# Set sources/includes
target_include_directories(ccli_header_only INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/ccli>"
                                                          "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# -----------------------------------------------------------------------------
# Development tools
# -----------------------------------------------------------------------------

if (CMAKE_BUILD_TYPE STREQUAL Debug)

    # Enable include-what-you-use
    if (CCLI_ENABLE_IWYU)
        # Setup repo and build binaries.
        include(cmake/ccli_configure_iwyu.cmake)
        ccli_setup_include_what_you_use(${CCLI_IWYU_CLANG_VERSION})

        # Enable include-what-you-use built in CMake support.
        set(IWYU_PATH "${CMAKE_SOURCE_DIR}/thirdparty/include-what-you-use/build/bin/include-what-you-use")
        set_target_properties(ccli PROPERTIES CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PATH})
    endif ()

endif ()

# -----------------------------------------------------------------------------
# Build binaries
# -----------------------------------------------------------------------------

if (CCLI_BUILD_EXAMPLES)
    message(STATUS "Generating Example(s)")
    add_subdirectory(examples)
endif ()

if (CCLI_BUILD_TESTS)
    message(STATUS "Generating tests")
    enable_testing()
    add_subdirectory(tests)
endif ()

# TODO: Add IDE support for headers. (source_group)
# TODO: CMake Install support.
# TODO: Header comments