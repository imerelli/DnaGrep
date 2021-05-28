/*
 * XXX THIS ONE NEEDS A MAJOR CLEANUP
 */

#ifndef __BayesModel_h
#define __BayesModel_h

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include "SequenceSet.h"

class BayesModel
  {
    int mError;
    int mErrorPosition;
    size_t mWidth;
	std::vector<std::vector<int> > mModel;
    std::vector<std::map<const std::string, REAL> > mScores;
	std::vector<REAL> mMissing;

	REAL mMinScore;
	REAL mFuzz;

public:
    // Error codes
    enum
      {
        kErrorNone = 0,
        kErrorInvalidSequence = 1,
        kErrorInvalidCharacter = 2,
        kErrorEmptyPattern = 3,
        kErrorExpectDigits = 4
      };

    BayesModel (std::string const& pattern, std::string const& examples, REAL fuzz = 2.0);
    BayesModel (const BayesModel &);
    BayesModel (void);
    ~BayesModel (void);

    BayesModel& invert () const;

    inline int getError (void) const
      {
        return mError;
      }

    inline int getErrorPosition (void) const
      {
        return mErrorPosition;
      }
    
    inline REAL getMinScore (void) const
      {
        return mMinScore;
      }
    
    inline size_t size (void) const
      {
        return mWidth;
      }

    void dump (std::ostream& o);

    template <typename T>
    inline REAL match (T const& t, REAL min, size_t shift = 0) const
      {
		REAL sco = 0.0;
		for(size_t i = 0; i < mModel.size() && sco > min; i++)
		{
		  std::string tmpString;
		  for(size_t j = 0; j < mModel[i].size(); j++)
			tmpString += t[mModel[i][j]+shift];
		  
		  std::map<const std::string, REAL>::const_iterator p = mScores[i].find(tmpString);
		  sco += ((p == mScores[i].end())?(mMissing[i]):((*p).second));
		}
		return sco;
      }

    template <typename T>
    inline REAL match (T const& t,  size_t shift = 0) const
      {
		return match(t, mMinScore, shift);
	  }
	  
	inline void dump(std::string filename = "-") const
	  {
	  
	  	std::ostream *of = &(std::cerr);
		std::ofstream outfile;
	  
		if(filename != "-")
		  {
			outfile.open(filename.c_str(), std::ios::binary | std::ios::out);
			of = & outfile;
	  	  }

	    (*of) << std::setprecision(15) << "DIAGNOSTIC DUMP FOLLOWS" << std::endl;
	    (*of) << "STATUS Error code         = " << mError <<  std::endl;
	    (*of) << "STATUS Error position     = " << mErrorPosition <<  std::endl;
	    (*of) << "STATUS Model width        = " << mWidth <<  std::endl;
	    (*of) << "STATUS Minimum score      = " << mMinScore <<  std::endl;
	    (*of) << "STATUS Bayesian fuzziness = " << mFuzz <<  std::endl;
	    (*of) << "STATUS Number of groups   = " << mModel.size() <<  std::endl;
	    (*of) << "STATUS Number of maps     = " << mScores.size() <<  std::endl;
	    (*of) << "STATUS Number of missing  = " << mMissing.size() <<  std::endl;

		for(size_t i = 0; i < mModel.size(); i++)
		  {
			(*of) << "GROUP (";
			for(size_t j = 0; j < mModel[i].size(); j++)
				(*of) << (j!=0?",":"") <<  mModel[i][j];
			(*of) << ") missing score " << mMissing[i] << std::endl;
		  }

		std::map<const std::string, REAL>::const_iterator p;
	
		for(size_t i = 0; i < mModel.size(); i++)
		  {
			for(p = mScores[i].begin(); p != mScores[i].end(); p++)
			  {
				(*of) << "SCORE ";
				std::string key = (*p).first;

				for(size_t j = 0; j < mModel[i].size(); j++)
				  {
					(*of) << mModel[i][j]+1 << key[j];
				  }
				
				(*of) << " === " << (*p).second << std::endl;
			  }
		  }

	  }
};

#endif  // !__BayesModel_h
