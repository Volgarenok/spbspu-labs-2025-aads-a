#include <fstream>
#include <iostream>
#include <stdexcept>
#include "args-parser.hpp"
#include "command-processor.hpp"
#include "io-utils.hpp"

int main(int argc, char** argv)
{
  using namespace kizhin;
  try {
    const Args args = parseArgs(argc, argv, std::cerr);
    const std::string& filename = args.filename;
    std::ifstream in(filename);
    if (!in.is_open()) {
      throw std::logic_error("Failed to open file: " + filename);
    }
    DSContainer datasets;
    if (!(in >> datasets)) {
      throw std::logic_error("Failed read data from file: " + filename);
    }
    processCommands(datasets, std::cin, std::cout);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}

