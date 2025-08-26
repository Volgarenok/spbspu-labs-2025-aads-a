#include <fstream>
#include <iostream>
#include <binary_tree/binary_tree.hpp>
#include "commands.hpp"
#include "key_summer.hpp"

int main(int argc, char **argv)
{
  using namespace abramov;

  if (argc != 3)
  {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  std::string mode(argv[1]);
  std::string file_name(argv[2]);
  std::ifstream file(file_name);
  if (!file)
  {
    std::cerr << "Wrong filename\n";
  }
  BinarySearchTree< long long int, std::string > tree{};
  long long int key = 0;
  std::string value;
  while (file >> key >> value)
  {
    tree.insert(key, value);
  }
  if (tree.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }
  CommandTree commands{};
  KeySummer summer{ 0, "" };
  getCommands(commands, tree, summer);
  try
  {
    commands[mode]();
    std::cout << summer.res << summer.val << "\n";
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what();
    return 1;
  }
  return 0;
}
