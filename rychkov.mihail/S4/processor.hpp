#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <string>
#include "map.hpp"
#include "parser.hpp"

namespace rychkov
{
  class S4ParseProcessor
  {
  public:
    S4ParseProcessor(int argc, char** argv);

    bool print(ParserContext& context);
    bool make_complement(ParserContext& context);
    bool make_intersect(ParserContext& context);
    bool make_union(ParserContext& context);
  private:
    using inner_map = rychkov::Map< int, std::string >;
    using outer_map = rychkov::Map< std::string, inner_map >;

    outer_map map;
  };
}

#endif
