#ifndef LISTUSINGS_H
#define LISTUSINGS_H
#include "fwdlst4d.hpp"
#include <utility>
#include <string>
using data_t = size_t;
using data_list_t = asafov::Forward_list<data_t>;
using sequence_t = std::pair<std::string, data_list_t>;
using sequence_list_t = asafov::Forward_list<sequence_t>;
#endif
