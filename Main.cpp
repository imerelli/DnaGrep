/*
 * This is our main invoker.
 * It contains a set of main_xxx() C style functions that wrap over
 * the C++ style Functions()
 * XXX: Major cleanup needed here too, make argument descriptors
 * a table and pass the arguments to Functions() as an hashmap
 */ 

#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>
#include "Usage.h"
#include "Profile.h"
#include "Scan.h"
#include "ModelScan.h"
#include "ModelStats.h"
#include "Stats.h"

int main_usage(int argc, char * const * argv)
  {
    std::string Program = argv[0];
    return Usage(Program);
  }

int main_profile(int argc, char * const * argv)
  {
    bool Verbose = false;
    bool Quiet = false;
    std::string Output = "-";

    int ch;

    while ((ch = getopt(argc, argv, "vqo:")) != -1)
      switch (ch) 
        {
          case 'v':
            Verbose = true;
            Quiet = false;
            break;
          case 'q':
            Quiet = true;
            Verbose = false;
            break;
          case 'o':
            Output = optarg;
            break;
          case '?':
          default:
            return main_usage(argc, argv);
         }
    argc -= optind;
    argv += optind;

    std::vector<std::string> List;

    for(int i = 0; i < argc; i++)
      List.push_back(argv[i]);

    return Profile(Verbose, Quiet, Output, List);
  }

int main_model(int argc, char * const * argv)
  {
    REAL Min = 0.0;
    bool Forward = true;
    bool Reverse = true;
    bool Mask = false;
    bool Annotation = true;
    bool Table = false;
    int Upstream = 20;
    int Downstream = 20;
    bool Verbose = false;
    bool Quiet = false;
    std::string Output = "-";
    std::string Input;

    int ch;
    while ((ch = getopt(argc, argv, "frbatvkqo:i:u:d:m:")) != -1)
      switch (ch) 
        {
          case 'k':
            Mask = true;
            break;
          case 'f':
            Forward = true;
            Reverse = false;
            break;
          case 'r':
            Forward = false;
            Reverse = true;
            break;
          case 'b':
            Forward = true;
            Reverse = true;
            break;
          case 'a':
            Annotation = true;
            Table = false;
            break;
          case 't':
            Table = true;
            Annotation = false;
            break;
          case 'v':
            Verbose = true;
            Quiet = false;
            break;
          case 'q':
            Quiet = true;
            Verbose = false;
            break;
          case 'o':
            Output = optarg;
            break;
          case 'u':
            Upstream = atoi(optarg);
            break;
          case 'd':
            Downstream = atoi(optarg);
            break;
          case 'm':
            Min = (REAL) atof(optarg);
            break;
          case '?':
          default:
            return main_usage(argc, argv);
         }

    if (argc-optind != 4)
      return main_usage(argc, argv);

    argc -= optind;
    argv += optind;

    std::string File = argv[0];
    --argc;
    ++argv;

    std::string Examples = argv[0];
    --argc;
    ++argv;

    std::string Model = argv[0];
    --argc;
    ++argv;

    std::string Pattern = argv[0];
    --argc;
    ++argv;

    return ModelScan(Forward, Reverse, Mask, Min, Annotation, Table, Upstream, Downstream, Verbose, Quiet, 
      Output, Pattern, File, Model, Examples);
  }


int main_scan(int argc, char * const * argv)
  {
    REAL Min = 0.0001;
    bool Forward = true;
    bool Reverse = true;
    bool Mask = false;
    bool Annotation = true;
    bool Table = false;
    int Upstream = 20;
    int Downstream = 20;
    bool Verbose = false;
    bool Quiet = false;
    std::string Output = "-";
    std::string Input;

    int ch;
    while ((ch = getopt(argc, argv, "frbatvkqo:i:u:d:m:")) != -1)
      switch (ch) 
        {
          case 'k':
            Mask = true;
            break;
          case 'f':
            Forward = true;
            Reverse = false;
            break;
          case 'r':
            Forward = false;
            Reverse = true;
            break;
          case 'b':
            Forward = true;
            Reverse = true;
            break;
          case 'a':
            Annotation = true;
            Table = false;
            break;
          case 't':
            Table = true;
            Annotation = false;
            break;
          case 'v':
            Verbose = true;
            Quiet = false;
            break;
          case 'q':
            Quiet = true;
            Verbose = false;
            break;
          case 'o':
            Output = optarg;
            break;
          case 'i':
            Input = optarg;
            break;
          case 'u':
            Upstream = atoi(optarg);
            break;
          case 'd':
            Downstream = atoi(optarg);
            break;
          case 'm':
            Min = (REAL) atof(optarg);
            break;
          case '?':
          default:
            return main_usage(argc, argv);
         }

    if (argc-optind < 1)
      return main_usage(argc, argv);

    argc -= optind;
    argv += optind;
    std::string File = argv[0];
    --argc;
    ++argv;

    std::vector<std::string> Patterns;
    for(int i = 0; i < argc; i++)
        Patterns.push_back(argv[i]);
 
    return Scan(Forward, Reverse, Mask, Min, Annotation, Table, Upstream, Downstream, Verbose, Quiet, 
      Output, Input, Patterns, File);
  }

int main_modelstats(int argc, char * const * argv)
  {
    bool Forward = true;
    bool Reverse = true;
    bool Mask = false;
    REAL Min = 0.0;
    bool Verbose = false;
    bool Quiet = false;
    int Buckets = 100;
    std::string Output = "-";

    int ch;
    while ((ch = getopt(argc, argv, "frtvkqo:i:m:n:")) != -1)
      switch (ch) 
        {
          case 'k':
            Mask = true;
            break;
          case 'f':
            Forward = true;
            Reverse = false;
            break;
          case 'r':
            Forward = false;
            Reverse = true;
            break;
          case 'b':
            Forward = true;
            Reverse = true;
            break;
          case 'v':
            Verbose = true;
            Quiet = false;
            break;
          case 'q':
            Quiet = true;
            Verbose = false;
            break;
          case 'n':
            Buckets = atoi(optarg);
            break;
          case 'o':
            Output = optarg;
            break;
          case 'm':
            Min = (REAL) atof(optarg);
            break;
          case '?':
          default:
            return main_usage(argc, argv);
         }

    if (argc-optind != 4)
      return main_usage(argc, argv);

    argc -= optind;
    argv += optind;

    std::string File = argv[0];
    --argc;
    ++argv;

    std::string Examples = argv[0];
    --argc;
    ++argv;

    std::string Model = argv[0];
    --argc;
    ++argv;

    std::string Pattern = argv[0];
    --argc;
    ++argv;

    return ModelStats(Forward, Reverse, Mask, Min, Verbose, Quiet, Buckets, Output,
		Pattern, File, Model, Examples);

}


int main_stats(int argc, char * const * argv)
  {
    REAL Min = 1e-6;
    bool Forward = true;
    bool Reverse = true;
    bool Mask = false;
    bool Verbose = false;
    bool Quiet = false;
    int Buckets = 100;
    bool Log = false;
    std::string Output = "-";
    std::string Input;

    int ch;
    while ((ch = getopt(argc, argv, "frblvkqo:i:n:m:")) != -1)
      switch (ch) 
        {
          case 'k':
            Mask = true;
            break;
          case 'f':
            Forward = true;
            Reverse = false;
            break;
          case 'r':
            Forward = false;
            Reverse = true;
            break;
          case 'b':
            Forward = true;
            Reverse = true;
            break;
          case 'l':
            Log = true;
            break;
          case 'v':
            Verbose = true;
            Quiet = false;
            break;
          case 'q':
            Quiet = true;
            Verbose = false;
            break;
          case 'o':
            Output = optarg;
            break;
          case 'i':
            Input = optarg;
            break;
          case 'n':
            Buckets = atoi(optarg);
            break;
          case 'm':
            Min = (REAL) atof(optarg);
            break;
          case '?':
          default:
            return main_usage(argc, argv);
         }

    if (argc-optind < 1)
      return main_usage(argc, argv);

    argc -= optind;
    argv += optind;
    std::string File = argv[0];
    --argc;
    ++argv;

    std::vector<std::string> Patterns;
    for(int i = 0; i < argc; i++)
        Patterns.push_back(argv[i]);
 
	return Stats(Forward, Reverse, Mask, Min, Verbose, Quiet, Buckets, Log, Output, Input, Patterns, File);

  }


int main(int argc, char * const * argv)
  {
    std::string Program = argv[0];

    if (argc > 1)
      {
        ++optind;
        std::string Command = argv[1];
        if(Command == "profile")
          return main_profile(argc, argv);
        else if(Command == "scan")
          return main_scan(argc, argv);
        else if(Command == "stats")
          return main_stats(argc, argv);
        else if(Command == "model")
          return main_model(argc, argv);
        else if(Command == "modstats")
          return main_modelstats(argc, argv);
      }

    return main_usage(argc, argv);
  }



