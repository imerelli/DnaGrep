#ifndef __ModelStats_h
#define __ModelStats_h

#include "config.h"
#include <iostream>
#include <vector>
#include <string>

extern int ModelStats(bool Forward, bool Reverse, bool Mask, REAL Min, bool Verbose, bool Quiet, int Buckets, std::string Output,
  std::string Pattern, std::string File, std::string Model, std::string Examples);

#endif  // !__ModelStats_h

