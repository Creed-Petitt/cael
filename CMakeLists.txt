cmake_minimum_required(VERSION 4.0)
project(cipr)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(cipr src/main.cpp
        src/Token/Token.h
        src/Scanner/Scanner.h
        src/Scanner/Scanner.cpp
        src/Core/Core.cpp
        src/Core/Core.h
        src/Token/Token.cpp
        src/AST/Node/Node.cpp
        src/AST/Node/Node.h
        src/AST/AstPrinter/AstPrinter.cpp
        src/AST/AstPrinter/AstPrinter.h
        src/Parser/Parser.cpp
        src/Parser/Parser.h
        src/Interpreter/Interpreter.cpp
        src/Interpreter/Interpreter.h
        src/Interpreter/Function.cpp
        src/Interpreter/Function.h
        src/Interpreter/Callable.h
        src/Common/Return.h
        src/Common/RuntimeError.h
        src/Native/NativeFunctions.h

        src/Environment/Environment.cpp
        src/Environment/Environment.h)
