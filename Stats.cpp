#include <fstream>
#include <math.h>
#include "Stats.h"
#include "FileReader.h"
#include "FastaParserWindowed.h"
#include "FuzzyPattern.h"

extern int Stats(bool Forward, bool Reverse, bool Mask, REAL Min, bool Verbose, bool Quiet, 
  int Buckets, bool Log, std::string Output, std::string Input,
  std::vector<std::string> Patterns, std::string File)
  {
    std::ofstream outfile;

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

    unsigned long int cnt[Buckets+1];
    for(int i = 0; i < Buckets+1; i++)
	  cnt[i]=0;    

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
				int wh = Buckets - 1 - (int) (((REAL)Buckets) *log(score)/log(Min));
				int wh2 = wh;
				if (wh2 < 0)
					wh2 = 0;
				if (wh2 > (Buckets -1))
					wh2 = Buckets-1;
				if(wh != wh2)
					std::cerr << "Mapped " << wh << " to " << wh2 << "\n";
				cnt[wh2]++;
              }
          }
    
        Windowed.get();
        if (Windowed.eos())
          Windowed.next();
      }

    for(int i = Buckets; i>0; i--)
		cnt[i-1] += cnt[i];

    for(int i = 0; i < Buckets; i++)
	{
		REAL score = exp(log(Min) * ( (REAL)(Buckets - 1 - i) ) / (REAL) Buckets);
		(*of) << score << "\t" << cnt[i] << "\t" << cnt[i]-cnt[i+1] << "\n";
	}


    if (of != &(std::cout))
      outfile.close();

    return 0;
  }

