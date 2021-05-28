#ifndef __ModelScan_h
#define __ModelScan_h

#include "config.h"
#include <iostream>
#include <vector>
#include <string>

extern int ModelScan(bool Forward, bool Reverse, bool Mask, REAL Min, bool Annotation, bool Table, int Upstream,
  int Downstream, bool Verbose, bool Quiet, std::string Output,
  std::string Pattern, std::string File, std::string Model, std::string Examples);

#endif  // !__ModelScan_h

