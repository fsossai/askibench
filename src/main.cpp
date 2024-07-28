#include <iostream>
#include <askiplot.hpp>
#include <cxxopts.hpp>

#include "version.hpp"
#include "askibench/benchmark.hpp"

using namespace std;
using namespace askiplot;

void printVersion();

int main(int argc, char *argv[]) {
  cxxopts::Options options("askibench", "askibench - Benchmark results directly in your console with ASCII characters");
  options.allow_unrecognised_options();
  options.show_positional_help();
  options.custom_help("[OPTION...] FILE...");
  options.add_options()
    ("t,tab", "Set TAB as the CSV delimiter")
    ("d,delimiter", "Set a specific char as CSV delimiter.", cxxopts::value<char>())
    ("W,width","Canvas maximum width. Assuming the width of the current console as default value.", cxxopts::value<int>())
    ("H,height","Canvas maximum width. Assuming the height of the current console as default value.", cxxopts::value<int>())
    ("brush-area","Set the character to be used for filling bars, or the area under curves.", cxxopts::value<char>()->default_value(askiplot::DefaultBrushArea))
    ("f,fill","Fill area under the curve. Use option --brush-area to set a custom char.")
    ("v,version","Display software version.")
    ("h,help","Display this help message.")
  ;

  try {
    auto args = options.parse(argc, argv);
    if (args.count("help")) {
      cout << options.help() << endl;
      return 0;
    }
    if (args.count("version")) {
      printVersion();
      return 0;
    }
    auto filenames = args.unmatched();
    if (filenames.size() != 0) {
      BarPlot barPlot;
      auto numGroups = filenames.size();
      auto barGrouper = BarGrouper(barPlot);

      for (auto filename : filenames) {
        auto benchmark = askibench::parseBenchmark(filename);
        auto medians = benchmark.medians();
        auto numThreads = benchmark.getNumThreads();
        barGrouper.Add(medians.flatten(), benchmark.getName());
      }

      barGrouper.Commit();
      barPlot.DrawLegend(East).DrawBarLabels(Offset(0, 1));
      cout << barPlot.Serialize();
    }
  } catch (const exception& e) {
    cout << "ERROR: ";
    cout << e.what() << endl;
    return 1;
  }

  return 0;
}

void printVersion() {
  cout <<
    "askibench version " <<
    to_string(askibench::version.major) << "." <<
    to_string(askibench::version.minor) << "." <<
    to_string(askibench::version.patch) << " (" <<
    ASKIBENCH_REPOSITORY_ORIGIN << " @ " <<
    ASKIBENCH_REPOSITORY_COMMIT << ")\n" <<
    "askiplot version " <<
    to_string(askiplot::version.major) << "." <<
    to_string(askiplot::version.minor) << "." <<
    to_string(askiplot::version.patch) << "\n"
    "cxxopts version " <<
    to_string(cxxopts::version.major) << "." <<
    to_string(cxxopts::version.minor) << "." <<
    to_string(cxxopts::version.patch) << "\n";
}
