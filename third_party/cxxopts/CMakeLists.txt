FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY  https://github.com/jarro2783/cxxopts
)
FetchContent_MakeAvailable(cxxopts)
FetchContent_GetProperties(cxxopts)

target_include_directories(askibench PUBLIC ${cxxopts_SOURCE_DIR}/include)
