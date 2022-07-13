cmake_minimum_required(VERSION 3.22)
project(config)

add_library(${PROJECT_NAME}
    include/${PROJECT_NAME}/Config.h
    src/Config.cpp

    src/Parse.h
    src/Parse.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME}
    PRIVATE doctest
    PRIVATE yaml-cpp
)
