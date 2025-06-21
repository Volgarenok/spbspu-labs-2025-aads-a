#ifndef IO_UTILS_HPP
#define IO_UTILS_HPP

#include <ios>
#include "infix-part.hpp"

namespace aleksandrov
{
  class StreamGuard
  {
  public:
    StreamGuard(std::basic_ios< char >&);
    ~StreamGuard();
  private:
    std::basic_ios< char >& s_;
    char fill_;
    std::streamsize precision_;
    std::streamsize width_;
    std::basic_ios< char >::fmtflags flags_;
  };

  std::istream& operator>>(std::istream&, InfixPart&);
}

#endif

