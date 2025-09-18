#include <fstream>
#include <iostream>
#include "io-utils.hpp"

int main(int argc, char** argv)
{
  if (argc != 2 || argv[1][0] == 0) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }
  const char* filename = argv[1];
  try {
    std::ifstream in(filename);
    if (!in.is_open()) {
      std::cerr << "Failed to open file: " << filename << '\n';
      return 1;
    }
    using namespace kizhin;
    GraphContainer graphs{};
    in >> graphs;
    processCommands(graphs, std::cin, std::cout);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}

