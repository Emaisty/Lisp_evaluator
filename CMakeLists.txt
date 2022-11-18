cmake_minimum_required(VERSION 3.15)
project(lisp_eval)

set(CMAKE_CXX_STANDARD 17)

add_executable(lisp_eval main.cpp Parser.h Parser.cpp AST.h AST.cpp Lexer.h Lexer.cpp)
