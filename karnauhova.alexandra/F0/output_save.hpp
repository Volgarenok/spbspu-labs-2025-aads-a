#ifndef OUTPUT_SAVE_HPP
#define OUTPUT_SAVE_HPP

#include <tree/avl_tree.hpp>
#include "characters.hpp"

namespace karnauhova
{
  void saveData(AvlTree< std::string, Character >& players, AvlTree< size_t, Character >& characters);
}
#endif
