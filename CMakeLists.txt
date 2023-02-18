cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(askibench)

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/third_party/cxxopts/include)
  message(FATAL_ERROR
    "Missing 'cxxopts' external dependency. "
    "Please run 'git submodule update --init' to fix the problem."
  )
endif()

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/third_party/AskiPlot/include)
  message(FATAL_ERROR
    "Missing 'AskiPlot' external dependency. "
    "Please run 'git submodule update --init' to fix the problem."
  )
endif()

add_executable(askibench ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_include_directories(askiplot PRIVATE
  ${CMAKE_SOURCE_DIR}/third_party/AskiPlot/include
  ${CMAKE_SOURCE_DIR}/third_party/cxxopts/include
)

set_property(TARGET askibench PROPERTY CXX_STANDARD 14)

