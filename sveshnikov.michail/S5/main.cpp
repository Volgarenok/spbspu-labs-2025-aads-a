#include <fstream>
#include <iostream>
#include <functional>
#include <tree/tree.hpp>
#include "kv-aggregator.hpp"
#include "commands.hpp"

sveshnikov::AvlTree< int, std::string > loadData(char *filename);

int main(int argc, char **argv)
{
  using namespace sveshnikov;
  if (argc != 3)
  {
    std::cerr << "ERROR: Not enough parameters!\n";
    return 1;
  }
  AvlTree< int, std::string > data;
  try
  {
    data = loadData(argv[2]);
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  KeyValAggr f;
  AvlTree< std::string, std::function< void(KeyValAggr &) > > command_holder;
  command_holder["ascending"] = std::bind(ascend< KeyValAggr & >, std::ref(data), std::ref(f));
  command_holder["descending"] = std::bind(descend< KeyValAggr & >, std::ref(data), std::ref(f));
  command_holder["breadth"] = std::bind(breadthTrav< KeyValAggr & >, std::ref(data), std::ref(f));
  try
  {
    command_holder.at(argv[1])(f);
    std::cout << f.key_sum_ << ' ' << f.values_ << '\n';
  }
  catch (const std::out_of_range &)
  {
    std::cerr << "ERROR: Invalid command!" << '\n';
    return 1;
  }
  catch (const std::logic_error &)
  {
    std::cout << "<EMPTY>" << '\n';
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  return 0;
}

sveshnikov::AvlTree< int, std::string > loadData(char *filename)
{
  using namespace sveshnikov;
  std::ifstream in(filename);
  if (!in.is_open())
  {
    throw std::runtime_error("ERROR: Cannot open file!");
  }
  AvlTree< int, std::string > data;
  int key = 0;
  std::string value;
  while (in >> key >> value)
  {
    data[key] = value;
  }
  if (!in.eof())
  {
    throw std::runtime_error("ERROR: Incorrect input!");
  }
  return data;
}
