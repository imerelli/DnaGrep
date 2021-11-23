#include <iomanip>
#include "config.h"
#include "BayesModel.h"
#include "SequenceSet.h"
#include <stdlib.h>

BayesModel::BayesModel (void)
  : mError (0),
    mErrorPosition (0)
{
}

enum
  {
      kStateInitial = 0,
      kStateNormal = 1,
      kStateGroup = 2,
      kStateDone = 3,
	  kStateMore = 4
  };

// The big contructor
BayesModel::BayesModel (std::string const& pattern, std::string const& examples, REAL fuzz)
  : mFuzz(fuzz)
{
  std::vector<std::map<const std::string, int> > mCounters;
  std::vector<int> mValidCounts;
  std::vector<int> mValid;
  SequenceSet mExamples;

  // Read in the examples file  
  mExamples.Read(examples, false, true);
  mExamples.CleanDNA();
  mWidth = mExamples.MaxLength();

  for(size_t i = 0; i < mWidth; i++)
  {
	int n = 0;
	for(size_t j = 0; j < mExamples.size(); j++)
	  if(mExamples[j][i] != '-')
	    n++;
	mValidCounts.push_back(n);
  }

  // Parse the model description
  int state = kStateInitial;
  std::string tmpString;
  std::vector<int> tmpVec;
  mError = 0;
  mErrorPosition = 0;

  
  for(size_t i = 0; i < pattern.length() && !mError ; i++)
  {

    char ch = pattern[i];
    
    switch (state)
      {
        case kStateInitial:
			mModel.clear();
            switch (ch)
              {
                case '(':
					tmpVec.clear();
                    state = kStateMore;
                  break;
                default:
                    tmpString.clear();
                    while((isdigit(ch) || (i == 0 && ch == '-')) && i < pattern.length())
                      {
                        tmpString += ch;
                        ch = pattern[++i];
                      }
                    if (i != 0)
                      {
						state = kStateDone;
						size_t m;
						int n = atoi(tmpString.c_str());
						if (n < 0)
						  {
						    n = 0-n;
							m = 0;
						  } else {
						    m = n++;
						  }
						  
						for (size_t s = m; s < (size_t) n; s++)
						  {
							// Build all possible subsets, I don't know how it works
							std::vector<int> tmpVec2(s, 0);
							size_t where = 1;

							while(where)
							  {
								where--;
								while(tmpVec2[where] < (int) mWidth)
								  {
									if((where+1)==s)
									  {
										mModel.push_back(tmpVec2);
										tmpVec2[where]++;
									  } else {
										where++;
										tmpVec2[where] = tmpVec2[where-1]+1;
									  }
								  }
								if(where)
									tmpVec2[where-1]++;
							  }
							}
                      } else {
						mError = kErrorExpectDigits;
						mErrorPosition = i;
					  }
                  break;
              }
          break;
        case kStateNormal:
            switch (ch)
              {
                case '(':
					tmpVec.clear();
                    state = kStateGroup;
                  break;
                default:
                    mError = kErrorInvalidCharacter;
                    mErrorPosition = i;
                  break;
              }
          break;
        case kStateGroup:
            switch (ch)
              {
                case ')':
					if (tmpVec.empty())
						{
							state = kStateDone;
							mError = kErrorEmptyPattern;
							mErrorPosition = i;
						} else {
						  mModel.push_back(tmpVec);
						  state = kStateNormal;
						}
				  break;
                case ',':
							state = kStateMore;
                  break;
                default:
				    if (isdigit(ch))
					  {
						tmpString.clear();
						while(isdigit(ch) && i < pattern.length())
						  {
							tmpString += ch;
							ch = pattern[++i];
						  }
						ch = pattern[--i];
						tmpVec.push_back(atoi(tmpString.c_str())-1);
						state = kStateGroup;
                      } else {
						mError = kErrorExpectDigits;
						mErrorPosition = i;
					  }
                  break;
              }
          break;
        case kStateMore:
            switch (ch)
              {
                default:
				    if (isdigit(ch))
					  {
						tmpString.clear();
						while(isdigit(ch) && i < pattern.length())
						  {
							tmpString += ch;
							ch = pattern[++i];
						  }
						ch = pattern[--i];
						tmpVec.push_back(atoi(tmpString.c_str())-1);
						state = kStateGroup;
                      } else {
						mError = kErrorExpectDigits;
						mErrorPosition = i;
					  }
                  break;
              }
          break;
        case kStateDone:
			mError = kErrorInvalidSequence;
		  break;
      }
  }

  // Count the combinations in the examples
  mCounters.resize(mModel.size());
  mValid.resize(mModel.size());
  for(size_t i = 0; i < mModel.size(); i++) // For all the groups in the model
  {
    mCounters[i].clear();
  
	// Number of valid sequences for this group in the exmaples
	size_t totvalid = 0;

	// Number of symbols in this group
	// size_t nct = mModel[i].size();

	// Let's go to count
	for(size_t j = 0; j < mExamples.size(); j++)  // For each sequence in the examples
	{
	  bool valid = true;
	  std::string tmpStr="";
	  for(size_t k = 0; k < mModel[i].size(); k++) // For each symbol in this group
		{
		  char ch = mExamples[j][mModel[i][k]]; // Fetch the symbol in the examples
		  if(ch != '-')
			tmpStr += ch;
		  else
		    valid = false;
		} // symbols
	  if(valid) {
	    mCounters[i][tmpStr]++;
		totvalid++;
	  }
	} // sequences
	mValid[i] = totvalid;

  } // groups

  // Pre-calculate all the scores
  std::map<const std::string, int>::iterator p;
  mScores.resize(mModel.size());
  mMissing.resize(mModel.size());
  mMinScore = 0.0;
  for(size_t i = 0; i < mModel.size(); i++)
	{
	  size_t pop = mValid[i]; // How many valid sequences exist for this group
	  size_t sym = mModel[i].size(); // How many symbols is this group made of

	  // Space == 4^sym
	  REAL space = 1.0;
	  for (size_t j = 0; j < sym; j++)
		space *= 4.0;

	  // Score for all missing combinations
	  mMissing[i] =  log((( mFuzz / space))/( ((REAL)pop) + mFuzz) );
	  mMinScore += mMissing[i];

	  for(p = mCounters[i].begin(); p != mCounters[i].end(); p++)
		{
		  std::string key = (*p).first; // A key (sequence) for this group
		  size_t cnt = (*p).second; // How many occurrences of this key
			
		  REAL score = log( ( ((REAL)cnt) + ( mFuzz / space))/( ((REAL)pop) + mFuzz) );
		  mScores[i][key] = score;
		}
	}
}

BayesModel::BayesModel(const BayesModel & fp)
	: mError(fp.mError),
	  mErrorPosition(fp.mErrorPosition),
	  mWidth(fp.mWidth),
	  mMinScore(fp.mMinScore),
	  mFuzz(fp.mFuzz)	
{
	size_t modelsize = fp.mModel.size();
	mModel.resize(modelsize);
	mScores.resize(modelsize);
	mMissing.resize(modelsize);

	for(size_t i = 0; i < modelsize; i++)
	{
		mMissing[i]=fp.mMissing[i];
		
		mModel[i].resize(fp.mModel[i].size());
		for(size_t j = 0; j < fp.mModel[i].size(); j++)
			mModel[i][j] = fp.mModel[i][j];
		
		for(std::map<const std::string, REAL>::const_iterator p = fp.mScores[i].begin(); p != fp.mScores[i].end(); p++)
			mScores[i][(*p).first] = (*p).second;
	}
}

BayesModel::BayesModel & BayesModel::invert () const
{
    BayesModel::BayesModel *res  = new BayesModel;

    res->mError = mError;
    res->mErrorPosition = mErrorPosition;
    res->mWidth = mWidth;
    res->mMinScore = mMinScore;
    res->mFuzz = mFuzz;

	size_t modelsize = mModel.size();

	res->mModel.resize(modelsize);
	res->mScores.resize(modelsize);
	res->mMissing.resize(modelsize);

	for(size_t i = 0; i < modelsize; i++)
	{
		res->mMissing[i]=mMissing[i];
		
		res->mModel[i].resize(mModel[i].size());
		for(size_t j = 0; j < mModel[i].size(); j++)
			res->mModel[i][j] = mWidth - 1 - (mModel[i][j]);
		
		for(std::map<const std::string, REAL>::const_iterator p = mScores[i].begin(); p != mScores[i].end(); p++)
		  {
			std::string tmpString = "";
			for(size_t j = 0; j < (*p).first.size(); j++)
			  {
				char ch = (*p).first[j];
				switch (ch)
				  {
					case 'a':
						ch = 't';
						break;
					case 'c':
						ch = 'g';
						break;
					case 'g':
						ch = 'c';
						break;
					case 't':
						ch = 'a';
						break;
				  }
				tmpString += ch;
			  }
			res->mScores[i][tmpString] = (*p).second;
		  }
	}

    return *res;
}

BayesModel::~BayesModel (void)
{
}

void BayesModel::dump (std::ostream& o)
{
}

