#include <exception>
#include <fstream>
#include <functional>
#include <limits>
#include "commands.hpp"

namespace
{
  void input(std::istream& in, kiselev::dataset& dictionary)
  {
    std::string name;
    std::string value;
    size_t key;
    while (in >> name)
    {
      kiselev::data tree;
      if (in.get() == '\n')
      {
        dictionary.insert(std::make_pair(name, tree));
        continue;
      }
      while (in >> key >> value)
      {
        tree.insert(std::make_pair(key, value));
        if (in.get() == '\n')
        {
          break;
        }
      }
      dictionary.insert(std::make_pair(name, tree));
    }
  }
}
int main(int argc, char** argv)
{
  using namespace kiselev;
  if (argc != 2)
  {
    std::cerr << "Invalid parameters\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  dataset dictionary;
  try
  {
    input(file, dictionary);
  }
  catch (const std::exception&)
  {
    std::cerr << "Error during input\n";
    return 1;
  }
  RBTree< std::string, std::function< void() > > commands;
  using namespace std::placeholders;
  commands.insert(std::make_pair("print", std::bind(print, std::ref(std::cout), std::ref(std::cin), std::cref(dictionary))));
  commands.insert(std::make_pair("complement", std::bind(complement, std::ref(std::cin), std::ref(dictionary))));
  commands.insert(std::make_pair("intersect", std::bind(intersect, std::ref(std::cin), std::ref(dictionary))));
  commands.insert(std::make_pair("union", std::bind(unite, std::ref(std::cin), std::ref(dictionary))));
  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)();
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
