/*
 * This is a generic File Reader.
 * Instantiate it passing a filename/path as constructor parameter and you
 * will be have get(), peek(), ftell(), eof(), etc.
 * The thing is one-byte-lookahead buffered for maximum performance (and
 * all inlined btw).
 * If you pass nothing or "-" as constructor parameter the data will come
 * from stdin.
 * If you pass a "seekable" object as constructor you will also be abe to
 * seek(). With stdin seek() is still available but only "forward", that
 * means to a position beyond the current ftell().
 * XXX TODO: Make a more decent buffering to not depend on xxx_unlocked()
 * and stdio library, in example use direct block-level file access....
 * XXX TODO2: Add support for on-the-fly decompression with gzlib.
 */

#ifndef __FileReader_h
#define __FileReader_h
#include <stdio.h>
#include <string>

class FileReader
  {
    FILE *mFile;
    int mNext;
    size_t mWhere;
    bool mSeek;

public:
    FileReader (void)
#ifdef LAME_UNNAMED_NAMESPACE
    : mFile(::stdin),
#else
    : mFile(stdin),
#endif
      mNext(0),
      mWhere(0),
      mSeek(false)
    {
      get();
      mWhere = 0;
    }
    
    FileReader (std::string const& name)
    : mNext(0),
      mWhere(0),
      mSeek(false)
    {
      if (name.length() == 0 || name == "-")
        {
#ifdef LAME_UNNAMED_NAMESPACE
          mFile = ::stdin;
#else
          mFile = stdin;
#endif       
          mSeek = false;
        }
      else
        {
          mFile = ::fopen(name.c_str(), "rb");
          mSeek = true;
        }

      if(mFile)
        get();
      mWhere = 0;
    }
    
    FileReader (char const *name)
    : mNext(0),
      mWhere(0),
      mSeek(false)
    {
      if (!name || !*name || (*name== '-' && name[1] == '\0'))
        {
#ifdef LAME_UNNAMED_NAMESPACE
          mFile = ::stdin;
#else
          mFile = stdin;
#endif       
          mSeek = false;
        }
      else
        {
          mFile = ::fopen(name, "rb");
          mSeek = true;
        }

      if(mFile)
        get();
      mWhere = 0;
    }
    
    inline int seek(size_t pos)
    {
      if (mSeek)
        {
          int ret = ::fseek(mFile, pos, SEEK_SET);
          get();
          return ret;
        }
      else
        {
          while (pos > mWhere)
            get();
          return (pos == mWhere ? 0 : -1);
        }
    } 

    inline int peek(void) const
    {
      return mNext;
    }

    inline int get(void)
    {
      int next = mNext;
      if (next != EOF)
        {
#ifdef HAVE_GETC_UNLOCKED        
          mNext = getc_unlocked(mFile);
#else
          mNext = getc(mFile);
#endif   
          mWhere++;
        }
        return next;
    }

    inline int eof(void) const
    {
      return mNext == EOF;
    }

    inline operator bool () const
    {
      return (mFile != NULL);
    }

    inline size_t ftell(void) const
    {
      if (mSeek)
        return (::ftell(mFile)-1);
      else
        return mWhere;
    }

    inline int error(void) const
    {
      return ::ferror(mFile);
    }
    
  };
  
#endif // __FileReader_h
