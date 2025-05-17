#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>
#include <fstream>
#include "calc_post.hpp"
#include "stack.hpp"
#include "queue.hpp"

namespace karnauhova
{
    karnauhova::Queue< std::string > split_str(std::string str);
    karnauhova::Stack< long long int > input_str(std::istream& in);
    karnauhova::Queue< std::string > to_post(karnauhova::Queue< std::string > inf);
}

#endif
