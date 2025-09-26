#include "key_sum.hpp"

void karnauhova::KeySum::operator()(const std::pair< const int, std::string > & key_value)
{
  result_ += key_value.first;
  if (!itInput_.empty())
  {
    itInput_ = itInput_ + " ";
  }
  itInput_ = itInput_ + key_value.second;
}
