#ifndef __Stats_h
#define __Stats_h

#include "config.h"
#include <iostream>
#include <vector>
#include <string>

extern int Stats(bool Forward, bool Reverse, bool Mask, REAL Min, bool Verbose, bool Quiet, 
  int Buckets, bool Log, std::string Output, std::string Input,
  std::vector<std::string> Patterns, std::string File);

#endif  // !__Stats_h

