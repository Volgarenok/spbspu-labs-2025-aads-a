#ifndef READ_SEQUENCES_HPP
#define READ_SEQUENCES_HPP
#include <iostream>
#include <string>
#include <list/list.hpp>

namespace smirnov
{
  void readSequences(std::istream & in, List< std::pair< std::string, List< size_t > > > & sequences);
  void newSequences(List< List< size_t > > & out, List< std::pair< std::string, List< size_t > > > & in);
}
#endif
