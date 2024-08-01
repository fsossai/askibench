#include <algorithm>
#include <askiplot.hpp>
#include <cxxopts.hpp>
#include <iostream>

#include "askibench/benchmark.hpp"
#include "version.hpp"

using namespace std;
using namespace askiplot;
using namespace askibench;

void PrintVersion();

int main(int argc, char *argv[]) {
  // clang-format off
  cxxopts::Options options("askibench", "askibench - Benchmark results directly in your console with ASCII characters");
  options.allow_unrecognised_options();
  options.show_positional_help();
  options.custom_help("[OPTION...] FILE...");
  options.add_options()
    ("W,width","Canvas maximum width. Assuming the width of the current console as default value.", cxxopts::value<int>())
    ("H,height","Canvas maximum width. Assuming the height of the current console as default value.", cxxopts::value<int>())
    ("s,speedup","Specify a file to use as a baseline for computing seepdups", cxxopts::value<string>())
    ("v,version","Display software version.")
    ("h,help","Display this help message.")
  ;
  // clang-format on

  try {
    auto args = options.parse(argc, argv);
    askiplot::BarValuePrecision = 0;
    if (args.count("speedup")) {
      askiplot::BarValuePrecision = 2;
    }
    if (args.count("help")) {
      cout << options.help() << endl;
      return 0;
    }
    if (args.count("version")) {
      PrintVersion();
      return 0;
    }
    int plot_width = 0;
    if (args.count("width")) {
      plot_width = std::max(0, args["width"].as<int>());
    }
    int plot_height = 0;
    if (args.count("height")) {
      plot_height = std::max(0, args["height"].as<int>());
    }

    auto filenames = args.unmatched();
    if (filenames.size() != 0) {
      BarPlot plot(plot_width, plot_height);
      plot.DrawBorders(Top).SetTitle(" AskiBench ").DrawTitle();

      benchmark_time_t baseline;
      if (args.count("speedup")) {
        auto file = args["speedup"].as<string>();
        auto benchmark = askibench::ParseBenchmark(file);
        auto geomeans = benchmark.Geomeans();

        // Finding a baseline.
        // The provided file must contain either the threads=1 configuration
        // or a unique configuration
        if (geomeans.Contains(1)) {
          baseline = geomeans[1][0];
        } else {
          if (geomeans.GetSize() > 1) {
            throw invalid_argument(
                "baseline for speedup has ambiguous configuration");
          } else {
            baseline = geomeans.Flatten()[0];
          }
        }
      }

      vector<benchmark_threads_t> thread_numbers;
      auto grouper = BarGrouper(plot);
      for (auto filename : filenames) {
        auto benchmark = askibench::ParseBenchmark(filename);
        auto tn = benchmark.GetThreadNumbers();

        // Looking for the highest number of bar groups
        if (tn.size() > thread_numbers.size()) {
          thread_numbers = std::move(tn);
        }

        Benchmark data;
        if (args.count("speedup")) {
          data = benchmark.Speedups(baseline).Geomeans();
        } else {
          data = benchmark.Medians();
        }
        grouper.Add(data.Flatten(), benchmark.GetName());
      }

      // Creating bar group names
      vector<string> group_names;
      for (const auto &x : thread_numbers) {
        group_names.push_back("threads=" + to_string((int)x));
      }

      grouper.SetGroupNames(group_names).Commit();
      plot.DrawBarLabels(Offset(0, 1)).DrawLegend(NorthEast);
      cout << plot.Serialize();
    }
  } catch (const exception &e) {
    cout << "ERROR: ";
    cout << e.what() << endl;
    return 1;
  }

  return 0;
}

void PrintVersion() {
  cout << "askibench version " << to_string(askibench::version.major) << "."
       << to_string(askibench::version.minor) << "."
       << to_string(askibench::version.patch) << " ("
       << ASKIBENCH_REPOSITORY_ORIGIN << " @ " << ASKIBENCH_REPOSITORY_COMMIT
       << ")\n"
       << "askiplot version " << to_string(askiplot::version.major) << "."
       << to_string(askiplot::version.minor) << "."
       << to_string(askiplot::version.patch)
       << "\n"
          "cxxopts version "
       << to_string(cxxopts::version.major) << "."
       << to_string(cxxopts::version.minor) << "."
       << to_string(cxxopts::version.patch) << "\n";
}
