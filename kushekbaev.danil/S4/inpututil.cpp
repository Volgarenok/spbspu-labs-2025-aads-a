#include "inpututil.hpp"

using dataset_t = kushekbaev::Tree< size_t, std::string >;
using dict_t = kushekbaev::Tree< std::string, dataset_t >;

dict_t kushekbaev::readInputFromFile(std::ifstream& in)
{
  if (!in.is_open())
  {
    throw std::runtime_error("File couldn't be open!");
  }
  dict_t inputdata;
  while (!in.eof())
  {
    std::string name;
    in >> name;
    size_t key = 0;
    std::string value;
    dataset_t tmp;
    char c = 0;
    while (in.get(c) && c != '\n')
    {
      in >> key >> value;
      tmp.insert(std::make_pair(key, value));
    }
    inputdata.insert(std::make_pair(name, std::move(tmp)));
  }
  return inputdata;
}
