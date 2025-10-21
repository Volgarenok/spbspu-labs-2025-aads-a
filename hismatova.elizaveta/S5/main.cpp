#include <iostream>
#include <cstring>
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
  tree.loadFromFile(filename);

  if (tree.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }

  long long sum = 0;
  bool overflow = false;
  bool first = true;

  auto process = [&](int key, const std::string& value)
  {
    long long newSum = sum + key;
    if ((key > 0 && newSum < sum) || (key < 0 && newSum > sum))
    {
      overflow = true;
    }
    sum = newSum;

    if (!first)
    {
      std::cout << " ";
    }
    std::cout << value;
    first = false;
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
    std::cerr << "\nError: integer overflow while summing keys\n";
    return 2;
  }

  std::cout << "\n" << sum << "\n";
  return 0;
}
