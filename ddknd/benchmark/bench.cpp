#include <benchmark/benchmark.h>
#include <vector>
#include <numeric>

static void BM_VectorSum(benchmark::State& state) {
  std::vector<int> v(state.range(0), 1);

  for (auto _ : state) {
    auto sum = std::accumulate(v.begin(), v.end(), 0);
    benchmark::DoNotOptimize(sum);
  }
}

BENCHMARK(BM_VectorSum)->Arg(1000)->Arg(100000);
BENCHMARK_MAIN();