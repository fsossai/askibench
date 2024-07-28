FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY  https://github.com/jarro2783/cxxopts
  GIT_TAG         v3.2.1
  GIT_SHALLOW     1
  GIT_PROGRESS    1
)
FetchContent_MakeAvailable(cxxopts)
FetchContent_GetProperties(cxxopts)

target_include_directories(askibench PUBLIC ${cxxopts_SOURCE_DIR}/include)
