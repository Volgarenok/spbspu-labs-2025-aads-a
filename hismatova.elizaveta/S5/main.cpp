#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>
#include "BinaryTree.hpp"

int main(int argc, char* argv[])
{
  using namespace hismatova;
  if (argc != 3)
  {
    std::cerr << "ERROR: invalid argc\n";
    return 1;
  }

  const char* mode = argv[1];
  const char* filename = argv[2];

  BinaryTree< int, std::string > tree;

  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Error: failed to open file\n";
    return 1;
  }

  int key;
  std::string value;
  bool loadError = false;

  while (file >> key >> value)
  {
    tree.insert(key, value);
  }

  if (file.fail() && !file.eof())
  {
    std::cerr << "Error: integer overflow while summing keys\n";
    return 2;
  }

  if (tree.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }

  long long sum = 0;
  std::string output;
  bool overflow = false;

  auto process = [&](int k, const std::string& val)
  {
    sum += k;
    if (sum > std::numeric_limits< int >::max() ||
        sum < std::numeric_limits< int >::min())
    {
      overflow = true;
    }

    if (!output.empty())
    {
      output += " ";
    }
    output += val;
  };

  if (strcmp(mode, "ascending") == 0)
  {
    tree.traverseAscending(process);
  }
  else if (strcmp(mode, "descending") == 0)
  {
    tree.traverseDescending(process);
  }
  else if (strcmp(mode, "breadth") == 0)
  {
    tree.traverseBreadth(process);
  }
  else
  {
    std::cerr << "Error: unknown traversal mode\n";
    return 1;
  }

  if (overflow)
  {
    std::cerr << "Error: integer overflow while summing keys\n";
    return 2;
  }
  std::cout << sum << " " << output << "\n";
  return 0;
}
