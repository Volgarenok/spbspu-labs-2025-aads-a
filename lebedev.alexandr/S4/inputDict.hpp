#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include <map>
#include <string>
#include <istream>

namespace lebedev
{

using dictOfDicts = std::map< std::string, std::map< int, std::string > >;
void inputDicts(std::istream& in, dictOfDicts& dict);

}
#endif
