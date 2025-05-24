#include <iostream>
#include "map.hpp"
#include "parser.hpp"
#include "processor.hpp"

int main(int argc, char** argv)
{
  using processor = rychkov::S4ParseProcessor;
  using parser_type = rychkov::Parser< processor >;
  parser_type::map_type call_map = {
    {"print", &processor::print},
    {"complement", &processor::make_complement},
    {"intersect", &processor::make_intersect},
    {"union", &processor::make_union}
  };
  parser_type parser{{std::cin, std::cout, std::cerr}, {}, std::move(call_map)};
  if (!parser.processor.init(parser.context, argc, argv))
  {
    return 1;
  }

  while (parser.run())
  {}
}
