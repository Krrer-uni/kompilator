cmake_minimum_required(VERSION 3.24.2)

project(kompilator)

set(CMAKE_CXX_STANDARD 17)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(CompilerParser parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
        DEFINES_FILE parser.hpp)
FLEX_TARGET(CompilerScanner lexer/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
        DEFINES_FILE lexer.hpp)
ADD_FLEX_BISON_DEPENDENCY(CompilerScanner CompilerParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(kompilator main.cc ${BISON_CompilerParser_OUTPUTS}
        ${FLEX_CompilerScanner_OUTPUTS} lexer/lexer.l variables.h compiler.h basic_blocks/basic_blocks.h
        basic_blocks/basic_blocks.cc compiler.cc variables.cc basic_blocks/expressions.cc basic_blocks/commands.cc basic_blocks/conditions.cc mult_div.h mult_div.cc)

target_link_libraries(kompilator ${FLEX_LIBRARIES} ${BISON_LIBRARIES})