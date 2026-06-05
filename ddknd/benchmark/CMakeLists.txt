include(FetchContent)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE BOOL "" FORCE)

# Declare and make Google Benchmark available
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.8.3 # Use a stable tag
)
FetchContent_MakeAvailable(benchmark)

set(ddknd_bench_source 
    bench.cpp
)

add_executable(ddknd_bench ${ddknd_bench_source})
target_link_libraries(ddknd_bench 
    PRIVATE
        benchmark::benchmark
        benchmark::benchmark_main
)

