#ifndef CALC_POST_HPP
#define CALC_POST_HPP

#include <limits>
#include <iostream>
#include "stack.hpp"
#include "queue.hpp"

namespace karnauhova
{
    long long int calculator(long long int first, long long int second, std::string operat);
    long long int proc_post(karnauhova::Queue< std::string > post);
}

#endif
