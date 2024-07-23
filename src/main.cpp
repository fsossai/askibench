#include <iostream>
#include <askiplot.hpp>
#include <cxxopts.hpp>

#include "version.hpp"

#define ASKIBENCH_VERSION_MAJOR 0
#define ASKIBENCH_VERSION_MINOR 1
#define ASKIBENCH_VERSION_PATCH 0

using namespace std;

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
    ("brush-line","Set the character to be used for drawing lines.", cxxopts::value<char>()->default_value(askiplot::DefaultBrushMain))
    ("brush-area","Set the character to be used for filling bars, or the area under curves.", cxxopts::value<char>()->default_value(askiplot::DefaultBrushArea))
    ("brush-empty","Set the character to be used as background filler.", cxxopts::value<char>()->default_value(askiplot::DefaultBrushBlank))
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
    if (filenames.size() == 0) {
      // TODO Assuming stdin as input
    } else if (filenames.size() > 1) {
      cerr <<
        "WARNING: More than one file name provided. "
        "Ignoring all except '" << filenames[0] << "'" << endl;
    }
  }
  catch (const exception& e) {
    cout << "ERROR: ";
    cout << e.what() << endl;
    return 1;
  }

  return 0;
}

void printVersion() {
  cout <<
    "askibench version " <<
    to_string(ASKIBENCH_VERSION_MAJOR) << "." <<
    to_string(ASKIBENCH_VERSION_MINOR) << "." <<
    to_string(ASKIBENCH_VERSION_PATCH) << " (" <<
    ASKIBENCH_REPOSITORY_ORIGIN << " @ " <<
    ASKIBENCH_REPOSITORY_COMMIT << ")\n" <<
    "askiplot version " <<
    to_string(askiplot::ASKIPLOT_VERSION_MAJOR) << "." <<
    to_string(askiplot::ASKIPLOT_VERSION_MINOR) << "." <<
    to_string(askiplot::ASKIPLOT_VERSION_PATCH) << "\n";
}
