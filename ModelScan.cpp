#include <fstream>
#include <iomanip>
#include "ModelScan.h"
#include "FileReader.h"
#include "FastaParserWindowed.h"
#include "FuzzyPattern.h"
#include "BayesModel.h"

extern int ModelScan(bool Forward, bool Reverse, bool Mask, REAL Min, bool Annotation, bool Table, int Upstream,
  int Downstream, bool Verbose, bool Quiet, std::string Output,
  std::string Pattern, std::string File, std::string Model, std::string Examples)
  {

	REAL Fuzz = 2.0;
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

	if(Verbose)
		std::cerr << "Now processing " << File << " " << std::endl;

    std::vector<FuzzyPattern> Matcher;
    std::vector<size_t> MatcherShift;
	std::vector<BayesModel> Scorer;
    std::vector<size_t> ScorerShift;
    std::vector<bool> Rev;

	FuzzyPattern fwd(Pattern);

	size_t maxsize, aScorerShift, aMatcherShift;

	BayesModel fwdsco(Model, Examples, Fuzz);

	if(Min == 0.0)
		Min = fwdsco.getMinScore();
/*
	BayesModel revsco = fwdsco.invert();
	fwdsco.dump("modellodritto.mdl");
	revsco.dump("modelloinvertito.mdl");
*/
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
		Rev.push_back(false);
	  }

	if(Reverse)
	  {
		Matcher.push_back(fwd.invert());
		Scorer.push_back(fwdsco.invert());
		MatcherShift.push_back(aMatcherShift);
		ScorerShift.push_back(aScorerShift);
		Rev.push_back(true);
	  }

    FileReader Reader(File);
    FastaParserWindowed Windowed(Reader, maxsize, Mask, true);

    while(!Windowed.eof())
      {
        for(size_t i = 0; i < Matcher.size(); i++)
			if(Matcher[i].match(Windowed, MatcherShift[i]) > 0.0)
			  {
				REAL score = Scorer[i].match(Windowed, Min, ScorerShift[i]);
				if (score > Min)
				{
				  (*of) << Windowed.name() << "\t";
				  (*of) << Windowed.tell() + 1 << "\t";
		                  (*of) << Windowed.tell() + 1 + maxsize << "\t";
				  if(Rev[i])
				    {
					  (*of) << Windowed.reverse(0,maxsize) << "\t" << "-" << "\t";
				    } else {
					  (*of) << Windowed.window(0,maxsize) << "\t" << "+" << "\t";
				    }
				   (*of) << std::setprecision(15) << score << std::endl;
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

