#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <map.hpp>
#include <safe_math.hpp>

int main(int argc, char** argv)
{
  if (argc != 3)
  {
    std::cerr << "wrong arguments count\n";
    return 1;
  }
  std::ifstream file(argv[2]);
  if (!file)
  {
    std::cerr << "failed to open file \"" << argv[2] << "\"\n";
    return 1;
  }
  rychkov::Map< int, std::string > map;
  int key = 0;
  std::string str;
  while (file >> key >> str)
  {
    if (!map.try_emplace(key, std::move(str)).second)
    {
      std::cerr << "key repeated\n";
      return 1;
    }
  }
  if (!file.eof())
  {
    std::cerr << "failed to read value pair\n";
    return 1;
  }

  struct Accumulator
  {
    int sum = 0;
    std::string str;
    void operator()(const std::pair< const int, std::string >& value)
    {
      sum = rychkov::safeAdd(sum, value.first);
      str += ' ';
      str += value.second;
    }
  };
  Accumulator acc;
  rychkov::Map< std::string, Accumulator (decltype(map)::*)(Accumulator) const > call_map = {
        {"ascending", &decltype(map)::traverse_lnr},
        {"descending", &decltype(map)::traverse_rnl},
        {"breadth", &decltype(map)::traverse_breadth}
      };

  decltype(call_map)::const_iterator call_p = call_map.find(argv[1]);
  if (call_p == call_map.cend())
  {
    std::cerr << "wrong traverse name\n";
    return 1;
  }
  try
  {
    acc = (map.*(call_p->second))(acc);
  }
  catch (const std::invalid_argument&)
  {
    std::cerr << "sum overflow\n";
    return 1;
  }
  if (map.empty())
  {
    std::cout << "<EMPTY>\n";
  }
  else
  {
    std::cout << acc.sum << acc.str << '\n';
  }
}
