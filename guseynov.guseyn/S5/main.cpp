#include <fstream>
#include <iostream>
#include <string>
#include "commands.hpp"
#include "key_summer.hpp"

int main(int argc, char* argv[])
{
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
    return 1;
  }
  guseynov::Tree< long long, std::string, std::less< long long > > tree;
  long long key = 0;
  std::string value;
  while (file >> key >> value)
  {
    tree.insert(std::make_pair(key, value));
  }
  if (tree.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }
  guseynov::CommandTree commands;
  guseynov::KeySummer summer;
  guseynov::getCommands(commands, tree, summer);
  try
  {
    auto command = commands.find(mode);
    if (command != commands.end())
    {
      command->second();
      std::cout << summer.result_ << summer.values_ << "\n";
    }
    else
    {
      std::cerr << "Invalid mode\n";
      return 1;
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what();
    return 1;
  }
  return 0;
}
