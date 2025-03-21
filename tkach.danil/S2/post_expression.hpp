#ifndef POST_EXPRESSION_HPP
#define POST_EXPRESSION_HPP

#include <string>
#include "queue.hpp"

namespace tkach
{
  struct PostExpression
  {
  public:

  private:
    Queue< std::string > expressions_;
  };
}

#endif
