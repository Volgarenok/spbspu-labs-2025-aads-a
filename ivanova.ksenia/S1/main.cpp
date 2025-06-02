#include <algorithm>
#include <iostream>
#include <string>
#include "list.hpp"

using ivanova::List;

struct NamedList
{
  std::string name;
  List<int> list;
  List<int>::iterator pos;

  NamedList(const std::string& name) : name(name) {}
};

int main()
{
}
