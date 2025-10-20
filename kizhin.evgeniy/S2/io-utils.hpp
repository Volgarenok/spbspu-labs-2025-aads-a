#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S2_IO_UTILS_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S2_IO_UTILS_HPP

#include <ios>
#include "postfix-expression.hpp"

namespace kizhin {
  std::istream& operator>>(std::istream&, Token&);
  PostfixExpression inputPostfixExpression(std::istream&);
  class StreamGuard;
}

class kizhin::StreamGuard
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

#endif

