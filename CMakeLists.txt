cmake_minimum_required (VERSION 3.15)

SET(APP_NAME ${APP_NAME_PARAM})
SET(BUILD_T ${BUILD_TYPE_PARAM})

SET(PROJECT_NAME ${APP_NAME}-${BUILD_T})

set(CMAKE_BUILD_TYPE ${BUILD_TYPE_PARAM})

set(CMAKE_CXX_STANDARD 17)

project (${PROJECT_NAME})

# Recurse into the "Hello" and "Demo" subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
include_directories(${CMAKE_SOURCE_DIR}/Parser ${CMAKE_SOURCE_DIR})
set(SRC_FILES   main.cpp 
                Parser/src/Parser.cpp 
                Parser/src/Tokenizer.cpp 
                Parser/src/Literal.cpp 
                Parser/src/Statement.cpp
                Parser/src/ExpressionStatement.cpp
                Parser/src/Expression.cpp
                Parser/src/BlockStatement.cpp)

add_executable (${PROJECT_NAME} ${SRC_FILES})