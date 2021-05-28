#include <fstream>
#include <iomanip>
#include "ModelStats.h"
#include "FileReader.h"
#include "FastaParserWindowed.h"
#include "FuzzyPattern.h"
#include "BayesModel.h"

extern int ModelStats(bool Forward, bool Reverse, bool Mask, REAL Min, bool Verbose, bool Quiet, int Buckets, std::string Output,
  std::string Pattern, std::string File, std::string Model, std::string Examples)
  {

	REAL Fuzz = 2.0;
	
    std::ofstream outfile;

    unsigned long int cnt[Buckets+1];
    for(int i = 0; i < Buckets+1; i++)
	  cnt[i]=0;    

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

	if(Verbose)
		std::cerr << "Now processing " << File << " " << std::endl;

    std::vector<FuzzyPattern> Matcher;
    std::vector<size_t> MatcherShift;
	std::vector<BayesModel> Scorer;
    std::vector<size_t> ScorerShift;

	FuzzyPattern fwd(Pattern);

	size_t maxsize, aScorerShift, aMatcherShift;

	BayesModel fwdsco(Model, Examples, Fuzz);

	if(Min == 0.0)
		Min = fwdsco.getMinScore();
	REAL Max = 0.0;

	BayesModel revsco = fwdsco.invert();

	if(fwdsco.size() > fwd.size())
	  {
		maxsize = fwdsco.size();
		aMatcherShift = (size_t)(fwdsco.size() - fwd.size());
		aScorerShift = 0;
	  } else {
		aScorerShift = (size_t)(fwd.size() - fwdsco.size());
		aMatcherShift = 0;
		maxsize = fwdsco.size();
	  }
	
	if(Forward) 
	  {
		Matcher.push_back(fwd);
		Scorer.push_back(fwdsco);
		MatcherShift.push_back(0);
		ScorerShift.push_back(0);
	  }

	if(Reverse)
	  {
		Matcher.push_back(fwd.invert());
		Scorer.push_back(fwdsco.invert());
		MatcherShift.push_back(aMatcherShift);
		ScorerShift.push_back(aScorerShift);
	  }

    FileReader Reader(File);
    FastaParserWindowed Windowed(Reader, maxsize, Mask, true);

    while(!Windowed.eof())
      {
        for(size_t i = 0; i < Matcher.size(); i++)
			if(Matcher[i].match(Windowed, MatcherShift[i]) > 0.0)
              {
				REAL score = Scorer[i].match(Windowed, Min, ScorerShift[i]);
				int wh = (int)( ((REAL) Buckets) * ( (score-Min)/(Max-Min) ) );
				int wh2 = wh;
				if (wh2 < 0)
					wh2 = 0;
				if (wh2 > (Buckets -1))
					wh2 = Buckets-1;
				if(wh != wh2)
					std::cerr << "Mapped " << wh << " to " << wh2 << " for " << score << std::endl;
				cnt[wh2]++;
              }
    
        Windowed.get();
        if (Windowed.eos())
          Windowed.next();
      }

	int tot = 0;
	for(int i = 0; i < Buckets; i++)
		tot += cnt[i];
	
    for(int i = 0; i < Buckets; i++)
	{
		REAL score = Min + ( ((REAL) i) * (Max-Min) / ((REAL) Buckets ));
		(*of) << score << "\t" << cnt[i] << "\t" << tot << "\n";
		tot -= cnt[i];
	}

    if (of != &(std::cout))
      outfile.close();

    return 0;
  }

