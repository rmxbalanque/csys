# CMake Project configuration was modeled after: https://github.com/gabime/spdlog

# Configure CMAKE Version.
cmake_minimum_required(VERSION 3.1...3.16)

# -----------------------------------------------------------------------------
# Start CCLI project.
# -----------------------------------------------------------------------------
include(cmake/ccli_utils.cmake)

# Configure library version
set(CCLI_MAJOR_VERSION 0)
set(CCLI_MINOR_VERSION 1)
set(CCLI_PATCH_VERSION 0)
set(CCLI_VERSION
        ${CCLI_MAJOR_VERSION}.${CCLI_MINOR_VERSION}.${CCLI_PATCH_VERSION})

# Add Custom In-Game CLI Project.
project(ccli VERSION ${CCLI_VERSION} LANGUAGES CXX)
message(STATUS "Build CCLI: ${PROJECT_VERSION}")

# Setup module subdirectory.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include(GNUInstallDirs)

# -----------------------------------------------------------------------------
# Set default build to debug
# -----------------------------------------------------------------------------

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose Release or Debug" FORCE)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_DEBUG_POSTFIX D)
endif()

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

# Install options
option(CCLI_INSTALL "Generate the install target" OFF)

# - Tools -

# Include-What-You-Use
option(CCLI_ENABLE_IWYU "Enables the usage of include what you use" OFF) # ON
set(CCLI_IWYU_CLANG_VERSION "9" CACHE STRING "Clang version that include-what-you-use will use") # 9

# Notify build type.
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

# -----------------------------------------------------------------------------
# Build Static/Shared Library
# -----------------------------------------------------------------------------

# ccli files
set(CCLI_SOURCES ${ccli_SOURCE_DIR}/src/ccli.cpp)
set(CCLI_HEADER_PATH "${ccli_SOURCE_DIR}/include/ccli")
set(CCLI_HEADERS
        "${CCLI_HEADER_PATH}/ccli.h"
        "${CCLI_HEADER_PATH}/ccli_autocomplete.h"
        "${CCLI_HEADER_PATH}/ccli_arguments.h"
        "${CCLI_HEADER_PATH}/ccli_command.h"
        "${CCLI_HEADER_PATH}/ccli_string.h"
        "${CCLI_HEADER_PATH}/ccli_system.h"
        "${CCLI_HEADER_PATH}/ccli_exceptions.h"
        "${CCLI_HEADER_PATH}/ccli_item.h"
        "${CCLI_HEADER_PATH}/ccli_argument_parser.h"
        "${CCLI_HEADER_PATH}/ccli_system.h")

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

# Library public include directories
target_include_directories(ccli PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
                                                      "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# Set precompiled headers if supported and enabled
if (COMMAND target_precompile_headers AND CCLI_ENABLE_PCH)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ccli_pch.h.in ${PROJECT_BINARY_DIR}/ccli_pch.h @ONLY)
    target_precompile_headers(ccli PRIVATE ${PROJECT_BINARY_DIR}/ccli_pch.h)
endif ()

# Export no symbols by default. (To ensure cross-compiler consistent behaviour)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# -----------------------------------------------------------------------------
# Header only version
# -----------------------------------------------------------------------------

# Add header version ccli
add_library(ccli_header_only INTERFACE)
add_library(ccli::ccli_header_only ALIAS ccli_header_only)

# Set sources/includes
target_include_directories(ccli_header_only INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
                                                          "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# Link ccli private dependencies

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

# -----------------------------------------------------------------------------
# Install CCLI
# -----------------------------------------------------------------------------

if (CCLI_INSTALL)

    message(STATUS "Generating install")

    # Install paths
    set(CONFIG_TARGETS_FILE "ccliConfig.cmake")
    set(VERSION_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/ccliConfigVersion.cmake")
    set(EXPORT_DEST_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/ccli")

    # -------------------------------------------------------------------------
    # Install targets
    # -------------------------------------------------------------------------
    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(TARGETS ccli ccli_header_only
            EXPORT ccli_targets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    # --------------------------------------------------------------------------
    # Install Config files and Export targets
    # --------------------------------------------------------------------------

    # Install CCLI Version config.
    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
            ${VERSION_CONFIG_FILE}
            VERSION ${CCLI_VERSION}
            COMPATIBILITY AnyNewerVersion
    )
    install(FILES "${VERSION_CONFIG_FILE}" DESTINATION "${EXPORT_DEST_DIR}")

    # Install CCLI Targets.
    install(EXPORT ccli_targets
            NAMESPACE ccli::
            FILE ${CONFIG_TARGETS_FILE}
            DESTINATION ${EXPORT_DEST_DIR})

endif()

# TODO: Add IDE support for headers. (source_group)
# TODO: Header comments