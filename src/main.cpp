#include <algorithm>
#include <askiplot.hpp>
#include <cxxopts.hpp>
#include <iostream>

#include "askibench/benchmark.hpp"
#include "version.hpp"

using namespace std;
using namespace askiplot;
using namespace askibench;

void printVersion();

int main(int argc, char *argv[]) {
  // clang-format off
  cxxopts::Options options("askibench", "askibench - Benchmark results directly in your console with ASCII characters");
  options.allow_unrecognised_options();
  options.show_positional_help();
  options.custom_help("[OPTION...] FILE...");
  options.add_options()
    ("W,width","Canvas maximum width. Assuming the width of the current console as default value.", cxxopts::value<int>())
    ("H,height","Canvas maximum width. Assuming the height of the current console as default value.", cxxopts::value<int>())
    ("b,baseline","Set a baseline", cxxopts::value<string>())
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
      printVersion();
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
      BarPlot barPlot(plot_width, plot_height);
      barPlot.DrawBorders(Top).SetTitle(" AskiBench ").DrawTitle();

      benchmark_time_t baseline;
      if (args.count("speedup")) {
        auto file = args["speedup"].as<string>();
        auto benchmark = askibench::parseBenchmark(file);
        auto geomeans = benchmark.geomeans();

        // Finding a baseline.
        // The provided file must contain either the threads=1 configuration
        // or a unique configuration
        if (geomeans.contains(1)) {
          baseline = geomeans[1][0];
        } else {
          if (geomeans.size() > 1) {
            throw invalid_argument(
                "baseline for speedup has ambiguous configuration");
          } else {
            baseline = geomeans.flatten()[0];
          }
        }
      }

      vector<benchmark_threads_t> threadNums;
      auto barGrouper = BarGrouper(barPlot);
      for (auto filename : filenames) {
        auto benchmark = askibench::parseBenchmark(filename);
        auto tn = benchmark.getNumThreads();

        // Looking for the highest number of bar groups
        if (tn.size() > threadNums.size()) {
          threadNums = std::move(tn);
        }

        Benchmark data;
        if (args.count("speedup")) {
          data = benchmark.speedups(baseline).geomeans();
        } else {
          data = benchmark.medians();
        }
        barGrouper.Add(data.flatten(), benchmark.getName());
      }

      // Creating bar group names
      vector<string> groupNames;
      for (const auto &x : threadNums) {
        groupNames.push_back("threads=" + to_string((int)x));
      }

      barGrouper.SetGroupNames(groupNames).Commit();
      barPlot.DrawBarLabels(Offset(0, 1)).DrawLegend(NorthEast);
      cout << barPlot.Serialize();
    }
  } catch (const exception &e) {
    cout << "ERROR: ";
    cout << e.what() << endl;
    return 1;
  }

  return 0;
}

void printVersion() {
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
