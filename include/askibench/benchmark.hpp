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
  const benchmark_data_t& getData() const;
  std::string getName() const;
  size_t size() const;
  std::vector<benchmark_threads_t> getNumThreads() const;

  // setters
  void setName(std::string name);

  Benchmark geomeans() const;
  Benchmark medians() const;
  Benchmark speedups(benchmark_time_t baseline) const;
  std::vector<benchmark_time_t> flatten() const;

  // operators
  std::vector<benchmark_time_t>& operator[](benchmark_threads_t threads);
  const std::vector<benchmark_time_t>& operator[](benchmark_threads_t threads) const;

  void print() const;
  bool contains(benchmark_threads_t threads) const;


private:
  benchmark_data_t threadsToTimes_;
  std::string name_;
};

Benchmark parseBenchmark(const std::string& inputFile);

} // namespace askibench
