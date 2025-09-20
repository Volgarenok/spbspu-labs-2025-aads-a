#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include "map-utils.hpp"

int main(int argc, char** argv)
{
  if (argc != 3 || argv[2][0] == '\0') {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " <ascending|descending|breadth> <filename>";
    return 1;
  }
  const char* filename = argv[2];
  using namespace kizhin;
  try {
    std::fstream in(filename);
    if (!in.is_open()) {
      std::cerr << "Failed to open file: " << filename << '\n';
      return 1;
    }
    MapT map;
    if (!(in >> map)) {
      std::cout << "<EMPTY>\n";
      return 0;
    }
    ValueCollector result;
    MapT* mapPtr = std::addressof(map);
    const Map< std::string, std::function< ValueCollector() > > commands{
      { "ascending", std::bind(&MapT::traverseLmr< ValueCollector >, mapPtr, result) },
      { "descending", std::bind(&MapT::traverseRml< ValueCollector >, mapPtr, result) },
      { "breadth", std::bind(&MapT::traverseBreadth< ValueCollector >, mapPtr, result) },
    };
    result = commands.at(argv[1])();
    std::cout << result.keys << ' ' << result.values << '\n';
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}

