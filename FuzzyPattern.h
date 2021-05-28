/*
 * XXX THIS ONE NEEDS A MAJOR CLEANUP
 */

#ifndef __FuzzyPattern_h
#define __FuzzyPattern_h

#include <string>
#include <iostream>

class FuzzyPattern
  {
    int mState;
    int mError;
    int mErrorPosition;
	bool mStrand;
    bool mCompiled;
    std::string mPattern;
	std::string mLabel;

private:
    static size_t const kEntries = 256;

    // Used to build the final array
    struct PatternEntry *mFirst, *mLast;

    // The final array used to match the pattern
    typedef REAL Array[kEntries];

    Array *mArrays;
    size_t mSize;

public:
    // Error codes
    enum
      {
        kErrorNone = 0,
        kErrorInvalidSequence,
        kErrorInvalidCharacter,
        kErrorEmptyPattern,
        kErrorExpectDigits
      };

public:
    FuzzyPattern (std::string const& pattern = std::string ());
    FuzzyPattern (const FuzzyPattern &);
    ~FuzzyPattern (void);

    FuzzyPattern& invert () const;

    void reset (void);

    inline std::string getPattern (void) const
      {
        return mPattern;
      }

    inline std::string getLabel (void) const
      {
        return mLabel;
      }

    inline bool getStrand (void) const
      {
        return mStrand;
      }

    inline operator bool (void) const
      {
        return isCompiled ();
      }
    
    inline bool isCompiled (void) const
      {
        return mCompiled;
      }
    
    inline int getError (void) const
      {
        return mError;
      }

    inline int getErrorPosition (void) const
      {
        return mErrorPosition;
      }
    
    inline size_t size (void) const
      {
        return mSize;
      }

    bool compile (std::string const& pattern);
    void dump (std::ostream& o);

    template <typename T>
    inline REAL match (T const& t, size_t shift = 0) const
      {
        REAL score = 1.0;

        for (size_t i = 0; i < mSize; i++)
          {
            int ch = tolower ( (int)t[i+shift]);
            if (!ch)
              return 0.0;
            score *= mArrays[i][ch];
          }

        return score;
      }

    template <typename T>
    inline REAL match (T const& t, REAL lim, size_t shift = 0) const
      {
        REAL score = 1.0;

        for (size_t i = 0; i < mSize; i++)
          {
            int ch = tolower ( (int)t[i+shift]);
            if (!ch)
              return 0.0;
            score *= mArrays[i][ch];
            if (score < lim)
              return 0.0;
          }

        return score;
      }

private:
    void mark (void); 
    bool pack (void);

    void insert (char ch, int score);
    void insert (std::string const& s, int score);

    void resize (size_t size);
  };

#endif  // !__FuzzyPattern_h
