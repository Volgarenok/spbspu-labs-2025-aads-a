#include <iostream>
#include <exception>
#include <parser.hpp>
#include "processor.hpp"

int main(int argc, char** argv)
{
  using processor = rychkov::S7ParseProcessor;
  rychkov::Parser::map_type< rychkov::S7ParseProcessor > call_map = {
        {"graphs", &processor::graphs},
        {"vertexes", &processor::vertexes},
        {"outbound", &processor::outbound},
        {"inbound", &processor::inbound},
        {"bind", &processor::bind},
        {"cut", &processor::cut},
        {"create", &processor::create},
        {"merge", &processor::merge},
        {"extract", &processor::extract}
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
