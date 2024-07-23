execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  OUTPUT_VARIABLE ASKIBENCH_REPOSITORY_COMMIT
  OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET
)

execute_process(
  COMMAND git remote get-url --all origin
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  OUTPUT_VARIABLE ASKIBENCH_REPOSITORY_ORIGIN
  OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET
)

configure_file(version.hpp.in version.hpp)

target_include_directories(askibench PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(askibench PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

install(DIRECTORY askibench DESTINATION include)
