
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include "cmds.hpp"

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "Error!! No enoughts arguments \n";
    return 1;
  }
  std::ifstream file(argv[1]);
  using namespace dribas;
  dataset datas;
  std::string name, value;
  size_t key;
  while (file >> name) {
    data tree;
    if (file.get() == '\n') {
      datas.insert(std::make_pair(name, tree));
      continue;
    }
    while (file >> key >> value) {
      tree.insert(std::make_pair(key, value));
      if (file.get() == '\n') {
        break;
      }
    }
    datas.insert(std::make_pair(name, tree));
  }
  file.close();

  dribas::AVLTree< std::string, std::function< void() > > cmds;
  cmds.insert(std::make_pair("print", std::bind(print, std::ref(std::cout), std::ref(std::cin), std::cref(datas))));
  cmds.insert(std::make_pair("complement", std::bind(complement, std::ref(std::cin), std::ref(datas))));
  cmds.insert(std::make_pair("intersect", std::bind(intersect, std::ref(std::cin), std::ref(datas))));
  cmds.insert(std::make_pair("union", std::bind(unite, std::ref(std::cin), std::ref(datas))));

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)();
    } catch (const std::exception&) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
