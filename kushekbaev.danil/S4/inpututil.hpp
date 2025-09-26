#ifndef INPUTUTIL_HPP
#define INPUTUTIL_HPP

#include <fstream>
#include "tree.hpp"

using dataset_t = kushekbaev::Tree< size_t, std::string >;
using dict_t = kushekbaev::Tree< std::string, dataset_t >;

namespace kushekbaev
{
  dict_t readInputFromFile(std::ifstream& in);
}

#endif
