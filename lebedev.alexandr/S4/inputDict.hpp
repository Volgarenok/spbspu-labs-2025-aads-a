#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include <string>
#include <istream>
#include <AVLtree.hpp>

namespace lebedev
{

using dictOfDicts = lebedev::AVLtree< std::string, lebedev::AVLtree< int, std::string > >;
void inputDicts(std::istream& in, dictOfDicts& dict);

}
#endif
