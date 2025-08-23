#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>
#include <fstream>
#include "calc_post.hpp"
#include "stack.hpp"
#include "queue.hpp"

namespace karnauhova
{
    Queue< std::string > splitStr(const std::string& str);
    Stack< long long int > inputStr(std::istream& in);
    Queue< std::string > toPost(Queue< std::string > inf);
    bool isOperator(const std::string& element);
}

#endif
