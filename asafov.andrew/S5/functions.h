#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <map.hpp>

int parseFile(const std::string& filename, asafov::map< int, std::string >& tree);
bool check_overflow(long long sum, int add);
int traverseTree(const std::string& mode, const asafov::map< int, std::string >& tree);

#endif
