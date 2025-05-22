#include "args-parser.hpp"
#include <cassert>
#include <cstring>
#include <ostream>

namespace kizhin {
  std::ostream& printUsage(std::ostream&, const char*);
}

kizhin::Args kizhin::parseArgs(const int argc, const char* const* const argv,
    std::ostream& usageOut)
{
  assert(argv && argv[0] && "argv is null or program name is missing");
  assert(argc > 0 && "argc must be positive");
  if (argc != 3 || argv[2][0] == '\0') {
    printUsage(usageOut, argv[0]) << '\n';
    std::exit(1);
  }
  const char* filename = argv[2];
  Args::TraverseT traverse = Args::ascending;
  if (std::strcmp(argv[1], "descending") == 0) {
    traverse = Args::descending;
  } else if (std::strcmp(argv[1], "breadth") == 0) {
    traverse = Args::breadth;
  } else if (std::strcmp(argv[1], "ascending") != 0) {
    printUsage(usageOut, argv[0]) << '\n';
    std::exit(1);
  };
  return Args{ filename, traverse };
}

std::ostream& kizhin::printUsage(std::ostream& out, const char* executablePath)
{
  assert(executablePath && "printUsage: nullptr instead of path");
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  out << "Usage: " << executablePath;
  out << " <ascending|descending|breadth> <filename>";
  return out;
}

