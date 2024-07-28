target_sources(
  askibench
  PRIVATE
  main.cpp
  benchmark.cpp
)
install(TARGETS askibench DESTINATION bin)
