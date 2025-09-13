#include "inputDict.hpp"

void lebedev::inputDicts(std::istream& in, dictOfDicts dict)
{
  std::string name = "";
  while (in)
  {
    in >> name;
    std::string data = "";
    int key = 0;
    std::map< int, std::string > temp;
    while (in && in.peek() != '\n')
    {
      in >> key >> data;
      temp.insert({ key, data });
    }
    dict.insert({ name, temp });
  }
  in.clear();
}
