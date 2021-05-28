#ifndef __Scan_h
#define __Scan_h

#include "config.h"
#include <iostream>
#include <vector>
#include <string>

extern int Scan(bool Forward, bool Reverse, bool Mask, REAL Min, bool Annotation, bool Table, int Upstream,
  int Downstream, bool Verbose, bool Quiet, std::string Output, std::string Input,
  std::vector<std::string> Patterns, std::string File);

#endif  // !__Scan_h

