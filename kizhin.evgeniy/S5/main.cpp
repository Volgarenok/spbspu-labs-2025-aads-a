#include <fstream>
#include <iostream>
#include <string>
#include "args-parser.hpp"
#include "map-utils.hpp"

int main(int argc, char** argv)
{
  using namespace kizhin;
  try {
    Args args = parseArgs(argc, argv, std::cerr);
    std::fstream in(args.filename);
    if (!in.is_open()) {
      throw std::logic_error("Failed to open file: " + args.filename);
    }
    MapT map;
    if (!(in >> map)) {
      std::cout << "<EMPTY>\n";
      return 0;
    }
    ValueCollector result;
    if (args.traverse == args.ascending) {
      result = map.traverseLmr(result);
    } else if (args.traverse == args.descending) {
      result = map.traverseRml(result);
    } else {
      result = map.traverseBreadth(result);
    }
    std::cout << result.keys << ' ' << result.values << '\n';
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}

