#ifndef MAIN_LIB_H
#define MAIN_LIB_H

#include <vector>

// Initializes glog, gtest, gflags, benchmark, and absl from `argc` and `argv`.
// Returns the unparsed arguments from `argv`.
std::vector<char*> InitMain(int argc, char** argv);

// Parses the unhandled flags from gflags and uses them to set absl flags.
// This can be used when linking with gtest's main but you also need to be able to
// use absl flags. This can be used within a TEST{,_F,_P}() method to set up those
// flags.
// TODO(@monkeynova): This is horrific in that we're poking through the
// internal abstraction to get at the original flags. Until all of ABSL,
// googletest and FBTD play nice together or I can use my own main, I don't see
// a different way to initialize absl flags.
void InitializeAbslFlagsFromGtest();

#endif  // MAIN_LIB_H
