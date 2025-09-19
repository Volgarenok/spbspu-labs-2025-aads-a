#ifndef BASE_HPP
#define BASE_HPP

#include <array.hpp>
#include <BiTree.hpp>
#include <string>
#include <memory>
#include "item.hpp"
#include "kit.hpp"
#include "knapsack.hpp"
#include <vector>
namespace averenkov
{
  class Base
  {
  public:
    Base();
    Array< std::shared_ptr< Item > > items;
    Tree< std::string, Kit > kits;
    Tree< std::string, Knapsack > knapsacks;
    Knapsack current_knapsack;
  };
}

#endif
