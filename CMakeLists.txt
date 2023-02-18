cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(askibench)

execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  OUTPUT_VARIABLE ASKIBENCH_REPOSITORY_COMMIT
  OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET
)
execute_process(
  COMMAND git remote get-url --all origin
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  OUTPUT_VARIABLE ASKIBENCH_REPOSITORY_REMOTE
  OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET
)
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/third_party/AskiPlot
  OUTPUT_VARIABLE ASKIPLOT_REPOSITORY_COMMIT
  OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET
)
execute_process(
  COMMAND git remote get-url --all origin
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/third_party/AskiPlot
  OUTPUT_VARIABLE ASKIPLOT_REPOSITORY_REMOTE
  OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET
)
configure_file(version.h.in version.h @ONLY)

add_executable(askibench ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_include_directories(askibench PRIVATE
  ${CMAKE_SOURCE_DIR}/third_party/AskiPlot/include
  ${CMAKE_SOURCE_DIR}/third_party/cxxopts/include
  ${CMAKE_CURRENT_BINARY_DIR}
)

set_property(TARGET askibench PROPERTY CXX_STANDARD 14)

