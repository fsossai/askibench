#include <csv.h>
#include <iostream>

#include "askibench/benchmark.hpp"

using namespace std;

namespace askibench {

void Benchmark::print() const {
  for (const auto &[threads, times] : threadsToTimes_) {
    cout << name_ << "."
         << "threads." << threads << " = [\n";
    for (auto t : times) {
      cout << "  " << t << "\n";
    }
    cout << "]\n";
  }
}

Benchmark Benchmark::medians() const {
  Benchmark medians = *this;

  for (auto [threads, times] : threadsToTimes_) {
    std::sort(times.begin(), times.end());
    auto mid = times.size() / 2LL;

    benchmark_time_t median;
    if (times.size() % 2 == 0) {
      median = (times[mid - 1] + times[mid]) / static_cast<benchmark_time_t>(2);
    } else {
      median = times[mid];
    }
    medians.threadsToTimes_[threads] = {median};
  }

  return medians;
}

string Benchmark::getName() const { return name_; }

vector<benchmark_threads_t> Benchmark::getNumThreads() const {
  vector<benchmark_threads_t> numThreads;
  for (const auto &[threads, _] : threadsToTimes_) {
    numThreads.push_back(threads);
  }
  return numThreads;
}

vector<benchmark_time_t> Benchmark::flatten() const {
  vector<benchmark_time_t> result;
  for (const auto &[threads, times] : threadsToTimes_) {
    for (auto t : times) {
      result.push_back(t);
    }
  }
  return result;
}

Benchmark parseBenchmark(const string &inputFile) {
  Benchmark benchmark;

  // trim extension if present
  size_t lastDot = inputFile.find_last_of(".");
  if (lastDot == string::npos) {
    benchmark.name_ = inputFile;
  } else {
    benchmark.name_ = inputFile.substr(0, lastDot);
  }

  io::CSVReader<2> in(inputFile.c_str());
  in.read_header(io::ignore_extra_column, "threads", "time");
  benchmark_threads_t threads;
  benchmark_time_t time;

  while (in.read_row(threads, time)) {
    benchmark.threadsToTimes_[threads].push_back(time);
  }

  return benchmark;
}

} // namespace askibench
