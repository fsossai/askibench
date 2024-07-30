cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

project(askibench)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_custom_target(
  CompileCommands ALL
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
)

add_executable(askibench)

target_compile_options(askibench PRIVATE -Wall -Wpedantic -Wextra)

add_subdirectory(third_party)
add_subdirectory(include)
add_subdirectory(src)
