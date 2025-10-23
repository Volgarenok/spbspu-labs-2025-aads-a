#ifndef INPUT_FILE_HPP
#define INPUT_FILE_HPP

#include <tree/avl_tree.hpp>
#include "characters.hpp"

namespace karnauhova
{
  AvlTree< std::string, Character > inputData(AvlTree< size_t, Character >& characters);
  void inputNewData(std::istream& in, AvlTree< size_t, Character >& characters);
  AvlTree< std::string, Character > inputSaveData(std::istream& in, AvlTree< size_t, Character >& characters);
}
#endif
