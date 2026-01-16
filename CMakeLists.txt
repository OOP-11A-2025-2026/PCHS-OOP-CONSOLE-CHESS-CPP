cmake_minimum_required(VERSION 3.15)
project(ConsoleChess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Generate compile_commands.json for IntelliSense

# Include headers from 'include/' folder
include_directories(include)

# Collect all source files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Build executable
add_executable(chess ${SOURCES})