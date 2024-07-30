#include <cmath>
#include <csv.h>
#include <iostream>

#include "askibench/benchmark.hpp"

using namespace std;

namespace askibench {

void Benchmark::print() const {
  for (const auto &[threads, times] : getData()) {
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

  for (auto &[threads, times] : medians.threadsToTimes_) {
    std::sort(times.begin(), times.end());
    auto mid = times.size() / 2LL;

    benchmark_time_t median;
    if (times.size() % 2 == 0) {
      median = (times[mid - 1] + times[mid]) / static_cast<benchmark_time_t>(2);
    } else {
      median = times[mid];
    }
    medians[threads] = {median};
  }
  return medians;
}

Benchmark Benchmark::geomeans() const {
  Benchmark geomeans;

  for (const auto &[threads, times] : getData()) {
    benchmark_time_t accumulator = 1;
    for (const auto &time : times) {
      accumulator *= time;
    }
    auto geomean = pow(accumulator, 1. / times.size());
    geomeans[threads] = {geomean};
  }
  return geomeans;
}

Benchmark Benchmark::speedups(benchmark_time_t baseline) const {
  Benchmark speedup;
  for (const auto &[threads, times] : getData()) {
    for (const auto &time : times) {
      speedup[threads].push_back(baseline / time);
    }
  }
  return speedup;
}

vector<benchmark_threads_t> Benchmark::getNumThreads() const {
  vector<benchmark_threads_t> numThreads;
  for (const auto &[threads, _] : getData()) {
    numThreads.push_back(threads);
  }
  return numThreads;
}

vector<benchmark_time_t> Benchmark::flatten() const {
  vector<benchmark_time_t> result;
  for (const auto &[threads, times] : getData()) {
    for (auto t : times) {
      result.push_back(t);
    }
  }
  return result;
}

void Benchmark::setName(string name) { name_ = name; }

string Benchmark::getName() const { return name_; }

Benchmark parseBenchmark(const string &inputFile) {
  Benchmark benchmark;

  // trim extension if present
  size_t lastDot = inputFile.find_last_of(".");
  if (lastDot == string::npos) {
    benchmark.setName(inputFile);
  } else {
    benchmark.setName(inputFile.substr(0, lastDot));
  }

  io::CSVReader<2> in(inputFile.c_str());
  in.read_header(io::ignore_extra_column, "threads", "time");
  benchmark_threads_t threads;
  benchmark_time_t time;

  while (in.read_row(threads, time)) {
    benchmark[threads].push_back(time);
  }

  return benchmark;
}

size_t Benchmark::size() const { return getData().size(); }

const benchmark_data_t &Benchmark::getData() const { return threadsToTimes_; }

vector<benchmark_time_t> &Benchmark::operator[](benchmark_threads_t threads) {
  return threadsToTimes_[threads];
}

const std::vector<benchmark_time_t> &
Benchmark::operator[](benchmark_threads_t threads) const {
  return threadsToTimes_.at(threads);
}

} // namespace askibench
