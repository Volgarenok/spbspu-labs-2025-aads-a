#ifndef TERMINAL_TEXT_HPP
#define TERMINAL_TEXT_HPP

#include <tree/avl_tree.hpp>
#include "characters.hpp"

namespace karnauhova
{
  void clearScreen();
  void helloWorld(AvlTree< std::string, Character >& players);
  void delay(int milliseconds);
}
#endif
