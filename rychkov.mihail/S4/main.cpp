#include <iostream>
#include <exception>
#include <parser.hpp>
#include "processor.hpp"

int main(int argc, char** argv)
{
  using processor = rychkov::S4ParseProcessor;
  rychkov::Parser::map_type< processor > call_map = {
        {"print", &processor::print},
        {"complement", &processor::make_complement},
        {"intersect", &processor::make_intersect},
        {"union", &processor::make_union}
      };

  try
  {
    processor proc{argc, argv};
    rychkov::ParserContext context{std::cin, std::cout, std::cerr};
    while (rychkov::Parser::parse(context, proc, call_map))
    {}
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
