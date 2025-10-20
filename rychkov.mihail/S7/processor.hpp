#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <string>
#include <boost/hash2/fnv1a.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include <unordered_map.hpp>
#include <set.hpp>
#include <parser.hpp>

namespace rychkov
{
  class S7ParseProcessor
  {
  public:
    S7ParseProcessor(int argc, char** argv);

    bool graphs(ParserContext& context);
    bool vertexes(ParserContext& context);
    bool outbound(ParserContext& context);
    bool inbound(ParserContext& context);
    bool bind(ParserContext& context);
    bool cut(ParserContext& context);
    bool create(ParserContext& context);
    bool merge(ParserContext& context);
    bool extract(ParserContext& context);
  private:
    using edge = std::pair< std::string, std::string >;
    using weights_set = MultiSet< unsigned >;
    using inner_map = UnorderedMap< edge, weights_set >;

    using vertexes_set = Set< std::string >;
    using outer_mapped_type = std::pair< inner_map, vertexes_set >;
    using outer_map = UnorderedMap< std::string, outer_mapped_type >;

    outer_map map;
  };
}

#endif
