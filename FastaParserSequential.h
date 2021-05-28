#ifndef __FastaParserSequential_h
#define __FastaParserSequential_h

#include "FastaParserInline.h"

class FastaParserSequential: public FastaParserInline
  {
    std::string mSequence;
    bool mEof;

public:

// Constructors
    FastaParserSequential(FileReader &reader, bool maskit = false, bool lower = false)
      : FastaParserInline(reader, maskit, lower),
        mSequence(std::string())
      {
        mEof = FastaParserInline::eof();
        while (!eos())
          mSequence += get();
      }

      inline bool next(void)
      {
        mSequence.clear();
        mEof = ! FastaParserInline::next();
        while (!eos())
          mSequence += get();
        return !mEof;
      }

    inline bool eof(void) const
      {
        return mEof;
      }

    inline std::string const & sequence(void) const
      {
        return mSequence;
      }

  };

#endif  // !__FastaParserSequential_h
