#ifndef OUTPUTLIST_HPP
#define OUTPUTLIST_HPP
#include <ostream>
#include <list>
#include <utility>
#include <stdexcept>

namespace dirti
{
  void outputList(const std::list< std::pair< std::string, std::list< size_t >>> mainList, std::ostream& out);
}

#endif
