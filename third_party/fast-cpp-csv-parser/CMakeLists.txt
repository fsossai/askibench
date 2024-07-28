find_package(Threads REQUIRED)

FetchContent_Declare(
  csvparser
  GIT_REPOSITORY  https://github.com/ben-strasser/fast-cpp-csv-parser.git
  GIT_SHALLOW     1
  GIT_PROGRESS    1
)
FetchContent_MakeAvailable(csvparser)
FetchContent_GetProperties(csvparser)

target_include_directories(askibench PUBLIC ${csvparser_SOURCE_DIR})
target_link_libraries(askibench PUBLIC Threads::Threads)
