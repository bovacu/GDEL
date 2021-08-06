cmake_minimum_required (VERSION 3.15)

SET(APP_NAME ${APP_NAME_PARAM})
SET(BUILD_T ${BUILD_TYPE_PARAM})
SET(RUN_TESTS ${RUN_TESTS})

SET(PROJECT_NAME ${APP_NAME}-${BUILD_T})

set(CMAKE_BUILD_TYPE ${BUILD_TYPE_PARAM})

set(CMAKE_CXX_STANDARD 17)

if(${RUN_TESTS} STREQUAL "RUN_TESTS")
    add_compile_definitions(RUN_TESTS)
endif()

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
                Parser/src/BlockStatement.cpp
                Parser/src/AdditiveExpression.cpp
                Parser/src/MultiplicativeExpression.cpp
                Parser/src/PrimaryExpression.cpp
                Parser/src/ParenthesisExpression.cpp
                Parser/src/AssigmentExpression.cpp
                Parser/src/TokenStripper.cpp
                Parser/src/Defines.cpp
                Parser/src/VariableStatement.cpp
                Parser/src/IfStatement.cpp
                Parser/src/LoopStatement.cpp
                Parser/src/ForLoopStatement.cpp
                Parser/src/RelationalExpression.cpp
                Parser/src/EqualityExpression.cpp
                Parser/src/LogicalAndExpression.cpp
                Parser/src/LogicalOrExpression.cpp
                Parser/src/UnaryExpression.cpp
                Parser/src/RangeExpression.cpp
                Parser/src/FunctionDeclaration.cpp
                Parser/src/MemberExpression.cpp
                Parser/src/CallExpression.cpp)

add_executable (${PROJECT_NAME} ${SRC_FILES})