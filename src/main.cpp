#include <iostream>

#include "askiplot/askiplot.hpp"
#include "cxxopts.hpp"

#define ASKIPLOT_CMDLINE_VERSION_MAJOR 0
#define ASKIPLOT_CMDLINE_VERSION_MINOR 1

using namespace std;

void print_version();

int main(int argc, char *argv[]) {
  cxxopts::Options options("askiplot", "AskiPlot - Lightweight ASCII plotter");
  options.add_options()
    ("t,tab", "Set TAB as the CSV delimiter")
    ("d,delimiter", "Set a specific char as CSV delimiter.", cxxopts::value<char>())
    ("W,width","Canvas maximum width. Assuming the width of the current console as default value.", cxxopts::value<int>())
    ("H,height","Canvas maximum width. Assuming the height of the current console as default value.", cxxopts::value<int>())
    ("brush-line","Set the character to be used for drawing lines.", cxxopts::value<char>()->default_value(askiplot::DefaultBrushMain))
    ("brush-area","Set the character to be used for filling bars, or the area under curves.", cxxopts::value<char>()->default_value(askiplot::DefaultBrushArea))
    ("brush-empty","Set the character to be used as background filler.", cxxopts::value<char>()->default_value(askiplot::DefaultBrushBlank))
    ("f,fill","Fill area under the curve. Use option --brush-area to set a custom char.")
    ("V,version","Display software version.")
    ("h,help","Display this help message.")
  ;

  try {
    auto args = options.parse(argc, argv);
    if (args.count("help")) {
      cout << options.help() << endl;
      return 0;
    }
    if (args.count("version")) {
      print_version();
      return 0;
    }
    auto filenames = args.unmatched();
    if (filenames.size() == 0) {
      // TODO Assuming stdin as input
    } else if (filenames.size() > 1) {
      // Too many files names provided
      cerr <<
        "WARNING: More than one file name provided. "
        "Ignoring all except '" << filenames[0] << "'" << endl;
    } else {
    }
  }
  catch (const exception& e) {
    cout << "ERROR: ";
    cout << e.what() << endl;
    return 1;
  }

  return 0;
}

void print_version() {
  cout <<
    "AskiPlot\n"
    "Library version: " <<
    to_string(ASKIPLOT_VERSION_MAJOR) << "." <<
    to_string(ASKIPLOT_VERSION_MINOR) << "." <<
    to_string(ASKIPLOT_VERSION_PATCH) << "\n" <<
    "Command-line tool version: " << 
    to_string(ASKIPLOT_CMDLINE_VERSION_MAJOR) << "." <<
    to_string(ASKIPLOT_CMDLINE_VERSION_MINOR) << "\n" <<
    "Repository: https://github.com/fsossai/AskiPlot\n"
    "This is free software; see the source for copying conditions.  There is NO\n"
    "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." <<
    endl;
}
