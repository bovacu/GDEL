cmake_minimum_required (VERSION 3.15)

SET(APP_NAME ${APP_NAME_PARAM})
SET(BUILD_T ${BUILD_TYPE_PARAM})
SET(RUN_TESTS ${RUN_TESTS})

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

SET(PROJECT_NAME ${APP_NAME}-${BUILD_T})

set(CMAKE_BUILD_TYPE ${BUILD_TYPE_PARAM})

set(CMAKE_CXX_STANDARD 17)

if(${RUN_TESTS} STREQUAL "RUN_TESTS")
    add_compile_definitions(RUN_TESTS)
endif()

project (${PROJECT_NAME})

include_directories(${CMAKE_SOURCE_DIR}/Parser ${CMAKE_SOURCE_DIR})
set(SRC_FILES   main.cpp 
                Parser/src/Defines.cpp
                Parser/include/TokenStripper.hpp
                Parser/include/Parser.hpp
            )

add_executable (${PROJECT_NAME} ${SRC_FILES})