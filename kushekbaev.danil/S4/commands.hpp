#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <iostream>
#include "UBST.hpp"

using dataset_t = kushekbaev::UBST< size_t, std::string >; //ЭТО СТРОКА!!
using dict_t = kushekbaev::UBST< std::string, dataset_t >; //ЭТО СЛОВАРЬ ИЗ СТРОК!!

namespace kushekbaev
{
  void print(std::ostream& out, std::istream& in, const dict_t& dictionary);

  void complement(std::istream& in, dict_t& dictionary);

  void intersect(std::istream& in, dict_t& dictionary);

  void unification(std::istream& in, dict_t& dictionary);
}

#endif
