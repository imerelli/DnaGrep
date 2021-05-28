#include <fstream>
#include "Scan.h"
#include "FileReader.h"
#include "FastaParserWindowed.h"
#include "FuzzyPattern.h"

extern int Scan(bool Forward, bool Reverse, bool Mask, REAL Min, bool Annotation, bool Table, int Upstream,
  int Downstream, bool Verbose, bool Quiet, std::string Output,  std::string Input,
  std::vector<std::string> Patterns, std::string File)
  {

    std::ofstream outfile;

    if(Table) // XXXXXXX Fix this
      std::cerr << "*** SORRY, ONLY CFF FORMAT SUPPORTED AS OUTPUT BY NOW\n";

	std::ostream *of = &(std::cout);
    if (Output == "-")
      {
        if (!Quiet)
          std::cerr << "Using standard output." << std::endl;
      }
    else
      {
        if (!Quiet)
          std::cerr << "Using " << Output << " for output." << std::endl;
          outfile.open(Output.c_str(), std::ios::binary | std::ios::out);
     	  of = & outfile;
      }

    if (!Input.empty())
      {
        if(Verbose)
          std::cerr << "Reading further patterns from " << Input << std::endl;

// XXX use something better here PLEASE
        std::ifstream inFile(Input.c_str(), std::ios::in);
        while(!inFile.eof())
          {
            char buf[4096];
            inFile.getline(buf, 4095);
            buf[4095] = 0;
            std::string pat = buf;
            if(!buf[0])
              continue;
            Patterns.push_back(buf);
            if(Verbose)
              std::cerr << "Added pattern " << buf << std::endl;
          }
      }

        if(Verbose)
          std::cerr << "Now processing " << File << " " << std::endl;

    std::vector<FuzzyPattern> Matcher;
    size_t maxsize = 0;

    for(size_t i = 0; i < Patterns.size(); i++)
      {
        FuzzyPattern fwd(Patterns[i]);
        if(maxsize < fwd.size())
          maxsize = fwd.size();
        if(Forward)
            Matcher.push_back(fwd);
        if(Reverse)
            Matcher.push_back(fwd.invert());
      }

    FileReader Reader(File);
    FastaParserWindowed Windowed(Reader, maxsize, Mask);

    while(!Windowed.eof())
      {
        for(size_t i = 0; i < Matcher.size(); i++)
          {
            REAL score = Matcher[i].match(Windowed, Min);

            if (score > 0.0)
              {
                (*of) << Windowed.name() << "\t";
                (*of) <<  Matcher[i].getLabel() << "\t";
                (*of) <<  score << "\t";
                (*of) << Windowed.tell() << "\t";
                (*of) << Windowed.tell() + Matcher[i].size() << "\t";
                (*of) << Windowed.window(0,Matcher[i].size()) << "\t";
                (*of) << (Matcher[i].getStrand() ? "+" : "-") <<  "\n";
              }
          }
    
        Windowed.get();
        if (Windowed.eos())
          Windowed.next();
      }

    if (of != &(std::cout))
      outfile.close();

    return 0;
  }

