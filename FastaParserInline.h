/*
 * Minimal Inline fasta File Format Reader
 * 
 * This class allows to parse/read on stream and on the fly a Fasta file 
 * format, input comes from a minimal FileReader object capable of get(), 
 * peek(), eof(), and _optionally_ seek() and ftell().
 * It retrieves or peeks one char at a time of the sequence, when the 
 * parser object is created it positions itself on the first char of the 
 * first sequence, then the sequence data can be "sniffed" with peak() or
 * retrieved one char at a time with get(), until eos() indicates that the
 * end of the sequence has been reached. 
 * The method next() moves the parser to the next sequence in the file, 
 * and returns !eof() thereafter; eof() tells if whe have reached the end
 * of the file.
 * At any moment name() and comment() give the details of the current 
 * sequence beeing parsed, and tell() gives the position of the current 
 * char in the sequence (logical position, that is number of bases/aminos 
 * from the beginning of the sequence).
 * If the FileReader used is seekable (that is provides seek/ftell) the 
 * parser state can be saved by recording ftell() (which simply wraps 
 * FileReader::ftell()) and stell() (which keep track of the phyisical 
 * byte position where the name of the current sequence began. These two 
 * values can be used then to create a new parser that restarts from the 
 * same point of the file, by means of the second constructor.
 *
 * FOR EFFICIENCY REASONS THIS SHOULD BETTER REMAIN ALL IN AN HEADER AND 
 * BE INLINED
 *
 */

#ifndef __FastaParserInline_h
#define __FastaParserInline_h

#include "FileReader.h"

class FastaParserInline
  {
    std::string mName;
    std::string mComment;
    size_t mPosition, mNameStart;
    FileReader &mReader;
    bool mEos;
	bool mMask;
	bool mLower;

public:
    // Builds a new parser from a generic reader
    FastaParserInline (FileReader &reader, bool maskit = false, bool lower = false)
      : mName(std::string()),
        mComment(std::string()),
        mPosition(0),
        mNameStart(0),
        mReader(reader),
        mEos(true),
		mMask(maskit),
		mLower(lower)
      {
        // Skip to the FIRST sequence !
        next();
      }

    inline bool eos(void) const
      {
        // Are we past the end of the current sequence ?
        return mEos;
      }

    inline bool eof(void) const
      {
        // Are we past the end of the file ?
        return mReader.eof();
      }

    inline std::string name(void) const
      {
        // What is the name of the current sequence ?
        return mName;
      }

    inline std::string comment(void) const
      {
        // What is the comment attached to the current sequnce ?
        return mComment;
      }

	inline int mask(int ch) const
	  {
	    if(mMask && islower(ch))
		  return '-';
		if (mLower)
			return tolower(ch);
		return ch;
	  }

    inline int peek(void) const
      {
        // Sniff the next aa/base of the sequence
        if (mEos)
          return EOF;
        else
          return mask(mReader.peek());
      }
    
    inline int get(void)
      {
        // Get the aa/base of the sequence
        if (mEos)
          return EOF;

        mPosition++;
        char ret = mask(mReader.get());
        char ch = ret;

        while ( !mEos && !isalpha(mReader.peek()) && mReader.peek() != '-' && mReader.peek() != '_' && mReader.peek() != '.' )
          {
            ch = mReader.get();
            mEos = ((mReader.peek() == '>' && (ch == '\n' || ch == '\r'))) || mReader.eof();
          }

        return ret;
      }

    inline size_t ftell() const
    {
      /// Retrn our current physical position in the FileReader
      return mReader.ftell();
    }

    inline size_t stell() const
    {
      // Return the pyisical position in the FileReader where this sequence's name began
      return mNameStart;
    }
     
    inline size_t tell() const
    {
      // Return the number of bases/aas already read from this sequence.
      return mPosition;
    }
     
    inline bool next(void)
      {
        // Go to the next sequence !
      
        // If we were not in the middle of a seq we are either on an EOL, or
        // at the beginning of the file or at EOF, in these places the following
        // loop can assume we are t an EOL.
        int ch = mEos ? '\n' : ' ';
        
        // Find the first EOL followed by a '>'
        while (((ch != '\n' && ch != '\r') || mReader.peek() != '>') && !mReader.eof())
          ch = mReader.get();

        // We are positioned on the '>'
        mNameStart = mReader.ftell();
        mReader.get(); // The '>'
        
        // Read the name of the sequence
        mName.erase();
        while (mReader.peek() != '\r' && mReader.peek() != '\n' && mReader.peek() != ' ' && !mReader.eof())
          mName += static_cast<char>(mReader.get());

        // Eat the space(s)
        while (mReader.peek() == ' ')
          mReader.get();

        // read the comment
        mComment.erase();
        while (mReader.peek() != '\r' && mReader.peek() != '\n' && !mReader.eof())
          mComment += static_cast<char>(mReader.get());

        // Find the beginning of the sequence, if any
        mEos = false;
        ch = mReader.peek();
        while ( !mEos && !isalpha(mReader.peek()) && mReader.peek() != '-' && mReader.peek() != '_' && mReader.peek() != '.' )
          {
            ch = mReader.get();
            mEos = ((mReader.peek() == '>' && (ch == '\n' || ch == '\r'))) || mReader.eof();
          }

        mPosition = 0;
        return !eof();

      }
  };

#endif  // !__FastaParserInline_h
