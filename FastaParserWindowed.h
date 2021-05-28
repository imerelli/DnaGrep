/*
 * This class extends the FastaParserInline by adding a "Window" of preview
 * on the following chars of the sequence.
 * Besides doing what a FastaParserInline you construct adding one more
 * parameter: the size of this window. Then you can look into the window
 * treating the FastaParserWindowed more or less as an array, this is done
 * by means of the ShiftRegister template class.
 * Once again performance requires inlining, and inlining requires that
 * this stays all into the header file.
 */

#ifndef __FastaParserWindowed_h
#define __FastaParserWindowed_h

#include "ShiftRegister.h"
#include "FastaParserInline.h"

typedef ShiftRegister <char> CharShiftRegister;

class FastaParserWindowed: public FastaParserInline
  {
    size_t mAvailable;
    CharShiftRegister mWindow;
    
public:

// Constructors
    FastaParserWindowed(FileReader &reader, size_t size, bool maskit = false, bool lower=false)
      : FastaParserInline (reader, maskit, lower),
        mAvailable(size),
        mWindow(size, '-')
      {
        for(size_t i = 0 ; i < size ; i++)
          get();
      }

      inline int get()
      {
        if (FastaParserInline::eos())
          {
            if(mAvailable > 0)
              {
                mAvailable--;
                return mWindow.shift_down('-');
              }
            else
              {
                return '-';
              }
          }
        else
          {
            return mWindow.shift_down(FastaParserInline::get());
          }
      }

      inline bool next(void)
      {
        mWindow.fill('-');
        mAvailable = mWindow.size();
        bool ret = FastaParserInline::next();
        for(size_t i = 0 ; i < mWindow.size() ; i++)
          get();
        return ret;
      }

      inline int peek (void) const
      {
        return mWindow[0];
      }
      
    inline char operator [] (ssize_t position) const
      {
        return mWindow[position];
      }

    inline size_t tell(void) const
      {
        return FastaParserInline::tell() - mAvailable;
      }

    inline bool eos(void) const
      {
        return mAvailable == 0;
      }

    inline bool eof(void) const
      {
        return eos() && FastaParserInline::eof();
      }

    inline std::string window(ssize_t pos = 0, ssize_t len = 0) const
      {
        std::string ret = "";
		if ((pos+len) > (ssize_t) mWindow.size())
			len = mWindow.size() - pos;
        for (size_t i = pos; len--; i++)
          ret += mWindow[i];
        return ret;
      }

    inline std::string reverse(ssize_t pos = 0, ssize_t len = 0) const
      {
        std::string ret = "";
		if ((pos+len) > (ssize_t) mWindow.size())
			len = mWindow.size() - pos;
        for (size_t i = mWindow.size()-1; len--; i--)
		  {
			char ch = mWindow[i];
			switch (ch)
			  {
				case 'a':
					ch = 't';
					break;
				case 'A':
					ch = 'T';
					break;
				case 'c':
					ch = 'g';
					break;
				case 'C':
					ch = 'G';
					break;
				case 'g':
					ch = 'c';
					break;
				case 'G':
					ch = 'C';
					break;
				case 't':
					ch = 'a';
					break;
				case 'T':
					ch = 'A';
					break;
			  }
			ret += ch;
		  }
        return ret;
      }
  };

#endif  // !__FastaParserWindowed_h
