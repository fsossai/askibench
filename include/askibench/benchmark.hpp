#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace askibench {

using benchmark_threads_t = int;
using benchmark_time_t = double;
using benchmark_data_t = std::map<benchmark_threads_t, std::vector<benchmark_time_t>>;

class Benchmark {
public:
  Benchmark() = default;
  ~Benchmark() = default;

  // getters
  const benchmark_data_t& GetData() const;
  std::string GetName() const;
  size_t GetSize() const;
  std::vector<benchmark_threads_t> GetThreadNumbers() const;

  // setters
  void SetName(std::string name);

  Benchmark Geomeans() const;
  Benchmark Medians() const;
  Benchmark Speedups(benchmark_time_t baseline) const;
  std::vector<benchmark_time_t> Flatten() const;

  // operators
  std::vector<benchmark_time_t>& operator[](benchmark_threads_t threads);
  const std::vector<benchmark_time_t>& operator[](benchmark_threads_t threads) const;

  void Print() const;
  bool Contains(benchmark_threads_t threads) const;


private:
  benchmark_data_t threads_to_times_;
  std::string name_;
};

Benchmark ParseBenchmark(const std::string& inputFile);

} // namespace askibench
