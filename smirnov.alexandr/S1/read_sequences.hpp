#ifndef READ_SEQUENCES_HPP
#define READ_SEQUENCES_HPP
#include <iostream>
#include <string>
#include <list>

namespace smirnov
{
  void readSequences(std::istream & in, std::list< std::pair< std::string, std::list< size_t > > > & sequences);
  void newSequences(std::list< std::list< size_t > > & out, std::list< std::pair< std::string, std::list< size_t > > > & in);
}
#endif
