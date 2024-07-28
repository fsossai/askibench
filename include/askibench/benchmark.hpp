#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace askibench {

using benchmark_threads_t = double;
using benchmark_time_t = double;

class Benchmark {
public:
  std::map<benchmark_threads_t, std::vector<benchmark_time_t>> threadsToTimes_;
  std::string name_;

  void print() const;
  Benchmark medians() const;
  std::string getName() const;
  std::vector<benchmark_threads_t> getNumThreads() const;
  std::vector<benchmark_time_t> flatten() const;
};

Benchmark parseBenchmark(const std::string& inputFile);

} // namespace askibench
