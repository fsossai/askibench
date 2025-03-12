#include <cmath>
#include <csv.h>
#include <iostream>

#include "askibench/benchmark.hpp"

using namespace std;

namespace askibench {

void Benchmark::Print() const {
  for (const auto &[threads, times] : GetData()) {
    cout << name_ << "."
         << "threads." << threads << " = [\n";
    for (auto t : times) {
      cout << "  " << t << "\n";
    }
    cout << "]\n";
  }
}

Benchmark Benchmark::Medians() const {
  Benchmark medians = *this;

  for (auto &[threads, times] : medians.threads_to_times_) {
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

Benchmark Benchmark::Geomeans() const {
  Benchmark geomeans;

  for (const auto &[threads, times] : GetData()) {
    benchmark_time_t accumulator = 0;
    for (const auto &time : times) {
      accumulator += log10(time);
    }
    auto geomean = pow(10., (double)accumulator / (double)times.size());
    geomeans[threads] = {geomean};
  }
  return geomeans;
}

Benchmark Benchmark::Speedups(benchmark_time_t baseline) const {
  Benchmark speedup;
  for (const auto &[threads, times] : GetData()) {
    for (const auto &time : times) {
      speedup[threads].push_back(baseline / time);
    }
  }
  return speedup;
}

Benchmark Benchmark::Speedups(const Benchmark& baseline) const {
  Benchmark speedup;
  auto geomean = baseline.Geomeans();
  for (const auto &[threads, times] : GetData()) {
    auto rbaseline = baseline[threads];
    for (const auto &time : times) {
      speedup[threads].push_back(geomean[threads][0] / time);
    }
  }
  return speedup;
}

vector<benchmark_threads_t> Benchmark::GetThreadNumbers() const {
  vector<benchmark_threads_t> thread_nums;
  for (const auto &[threads, _] : GetData()) {
    thread_nums.push_back(threads);
  }
  return thread_nums;
}

vector<benchmark_time_t> Benchmark::Flatten() const {
  vector<benchmark_time_t> result;
  for (const auto &[threads, times] : GetData()) {
    for (auto t : times) {
      result.push_back(t);
    }
  }
  return result;
}

void Benchmark::SetName(string name) { name_ = name; }

string Benchmark::GetName() const { return name_; }

Benchmark ParseBenchmark(const string &input_file) {
  Benchmark benchmark;

  // trim extension if present
  size_t lastDot = input_file.find_last_of(".");
  if (lastDot == string::npos) {
    benchmark.SetName(input_file);
  } else {
    benchmark.SetName(input_file.substr(0, lastDot));
  }

  io::CSVReader<2> in(input_file.c_str());
  in.read_header(io::ignore_extra_column, "threads", "time");
  benchmark_threads_t threads;
  benchmark_time_t time;

  while (in.read_row(threads, time)) {
    benchmark[threads].push_back(time);
  }

  return benchmark;
}

size_t Benchmark::GetSize() const { return GetData().size(); }

bool Benchmark::Contains(benchmark_threads_t threads) const {
  return threads_to_times_.find(threads) != threads_to_times_.end();
}

const benchmark_data_t &Benchmark::GetData() const { return threads_to_times_; }

vector<benchmark_time_t> &Benchmark::operator[](benchmark_threads_t threads) {
  return threads_to_times_[threads];
}

const std::vector<benchmark_time_t> &
Benchmark::operator[](benchmark_threads_t threads) const {
  return threads_to_times_.at(threads);
}

} // namespace askibench
