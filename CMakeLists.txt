cmake_minimum_required(VERSION 3.24)
project(ut-examples-jp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# clang-tidy: export compile-setting to compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(examples)