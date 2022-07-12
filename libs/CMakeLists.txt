# Apply patches
message("Apply patches")
find_package(Git REQUIRED)

execute_process(COMMAND sh ./scripts/apply-patches.sh
    RESULT_VARIABLE APPLY_PATCHES_CODE
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

if(NOT APPLY_PATCHES_CODE EQUAL 0)
    message(FATAL_ERROR "Can't apply patches (code: ${APPLY_PATCHES_CODE})")
endif()

# doctest
set(DOCTEST_NO_INSTALL ON CACHE INTERNAL "" FORCE)
add_subdirectory(doctest)

# cxxopts
add_subdirectory(cxxopts)

# yaml-cpp
add_subdirectory(yaml-cpp)

# libgit2
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "" FORCE)
set(BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(BUILD_CLI OFF CACHE INTERNAL "" FORCE)
add_subdirectory(libgit2)
