#ifndef MAIN_LIB_H
#define MAIN_LIB_H

#include <string_view>
#include <vector>

std::vector<char*> InitMain(int argc, char** argv, std::string_view usage);

#endif  // MAIN_LIB_H
