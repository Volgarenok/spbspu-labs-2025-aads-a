#include <iostream>
#include <fstream>
#include <tree.hpp>
#include "KeyValue.hpp"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Invalid arguments\n";
    return 1;
  }
  std::string mode(argv[1]);
  std::string filename(argv[2]);
  duhanina::Tree< long long, std::string, std::less< long long > > tree;
  std::ifstream file(filename);
  if (!file)
  {
    std::cerr << "Invalid file\n";
    return 1;
  }
  long long key = 0;
  std::string value;
  while (file >> key >> value)
  {
    tree.insert({ key, value });
  }
  if (tree.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }
  try
  {
    duhanina::KeySum key_sum;
    if (mode == "ascending")
    {
      key_sum = tree.traverse_lnr(key_sum);
    }
    else if (mode == "descending")
    {
      key_sum = tree.traverse_rnl(key_sum);
    }
    else if (mode == "breadth")
    {
      key_sum = tree.traverse_breadth(key_sum);
    }
    else
    {
      std::cerr << "Invalid mode\n";
      return 1;
    }
    std::cout << key_sum.key << " " << key_sum.value << "\n";
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  return 0;
}
