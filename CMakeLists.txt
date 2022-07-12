cmake_minimum_required(VERSION 3.22)
project(vanity)

# Install directory
set(INSTALL_DIR ${CMAKE_BINARY_DIR}/install)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${INSTALL_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${INSTALL_DIR})

# Global target compile options
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(TARGET_COMPILE_OPTIONS
        -Wall
        -Wextra
        -Werror
        -Wpedantic
        -Wall
        -Wextra
        -Werror
        -Wpedantic
        -Wshadow
        -Wredundant-decls
        -Wunused
        -Wdate-time
        -fno-extended-identifiers
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")

    set(TARGET_COMPILE_OPTIONS
        -Wall
        -Wextra
        -Werror
        -Wpedantic
        -Wshadow-all
        -Wunused
        -Wunused-member-function
        -Wdate-time
        -Wthread-safety
        -Wloop-analysis
        -Wconditional-uninitialized
        -Wsuggest-override
        -Wunreachable-code-aggressive
        -Wimplicit-fallthrough
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(TARGET_COMPILE_OPTIONS /W4 /WX /utf-8)
endif()

# Global target properties
set(TARGET_PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Testing
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPERCASED)
option(${${PROJECT_NAME_UPPERCASED}_BUILD_TESTS} "${PROJECT_NAME}: Build tests" OFF)
if(${${PROJECT_NAME_UPPERCASED}_BUILD_TESTS})
    enable_testing()
    add_test(NAME ${PROJECT_NAME}-test COMMAND app)
else()
    add_compile_definitions(DOCTEST_CONFIG_DISABLE)
endif()

# 3rd-party libs
add_subdirectory(libs)

# Internal libs
add_subdirectory(src)
