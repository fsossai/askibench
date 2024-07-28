FetchContent_Declare(
  askiplot
  GIT_REPOSITORY  https://github.com/fsossai/AskiPlot
  GIT_SHALLOW     1
  GIT_PROGRESS    1
)
FetchContent_MakeAvailable(askiplot)
FetchContent_GetProperties(askiplot)

target_include_directories(askibench PUBLIC ${askiplot_SOURCE_DIR}/include)
