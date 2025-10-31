#ifndef INTERACTION_GAME_HPP
#define INTERACTION_GAME_HPP

#include <tree/avl_tree.hpp>
#include "characters.hpp"

namespace karnauhova
{
  void choiceCharact(AvlTree< std::string, Character >& players, const AvlTree< size_t, Character >& characters);
  void fight(AvlTree< std::string, Character >& players);
}
#endif
