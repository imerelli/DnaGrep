#include "config.h"
#include "FuzzyPattern.h"

static size_t const kEntries = 256;

struct PatternEntry
  {
    PatternEntry *mNext;
    int mArray[kEntries];
  };

enum
  {
    kStateNormal = 0,
    kStateScore = 1
  };

FuzzyPattern::FuzzyPattern (std::string const& pattern)
  : mStrand (true),
	mFirst (NULL),
    mArrays (NULL),
    mSize (0)
{
  compile (pattern);
}

FuzzyPattern::FuzzyPattern(const FuzzyPattern & fp)
  : mState(fp.mState),
    mError(fp.mError),
    mErrorPosition(fp.mErrorPosition),
	mStrand(fp.mStrand),
    mCompiled(fp.mCompiled),
    mPattern(fp.mPattern),
	mLabel(fp.mLabel),
    mFirst(NULL),
    mLast(NULL),
    mArrays(NULL),
    mSize(0)
{
  resize(fp.mSize);
  mSize = fp.mSize;

  for (size_t i = 0; i < mSize; i++)
      for (size_t m = 0; m < kEntries; m++)
        mArrays[i][m] = fp.mArrays[i][m];
}  

FuzzyPattern::FuzzyPattern &
FuzzyPattern::invert () const
{
  FuzzyPattern::FuzzyPattern *res  = new FuzzyPattern;
  res->mLabel = mLabel;
  res->mStrand = !mStrand;

  if(mCompiled)
    {
      res->reset ();
      res->resize (mSize);
      res->mCompiled = true;

      for (size_t i = 0; i < mSize; i++)
        for (size_t m = 0; m < kEntries; m++)
          {
            size_t c;
            switch (m)
              {
                case 'a':
                case 'A':
                  c = 't';
                  break;

                case 't':
                case 'T':
                  c = 'a';
                 break;

                case 'g':
                case 'G':
                  c = 'c';
                  break;

                case 'c':
                case 'C':
                  c = 'g';
                  break;

                default:
                  c = m;
                  break;
              }
  
            res->mArrays[mSize - i - 1][c] = mArrays[i][m];
          }
    }
  return *res;
}

FuzzyPattern::~FuzzyPattern (void)
{
  reset ();
}

void FuzzyPattern::reset (void)
{
  PatternEntry *entry = mFirst;
  while (entry)
    {
      PatternEntry *next = entry->mNext;
      delete entry;
      entry = next;
    }

  mFirst = mLast = NULL;
  mCompiled = false;
  mError = kErrorNone;
  mErrorPosition = 0;
  mPattern.empty ();
  mLabel.empty();
  resize (0); // Destroy mArrays
}

void FuzzyPattern::mark (void)
{
  PatternEntry *entry;
//  std::cout << "Mark" << std::endl;

  entry = new PatternEntry;
  memset (entry, 0, sizeof (PatternEntry));

  if (! mFirst)
    {
      mFirst = entry;
      mLast = mFirst;
    }
  else
    {
      mLast->mNext = entry;
      mLast = mLast->mNext;
    }
}

void FuzzyPattern::insert (char ch, int score)
{
//  std::cout << "Inserting (" << ch << ") = " << (int)score << std::endl;
  mLast->mArray[(unsigned char) ch] = score;
}

void FuzzyPattern::dump (std::ostream& o)
{
  PatternEntry *entry;
  bool flag;

  for (entry = mFirst; entry != mLast; entry = entry->mNext)
    {
      flag = false;
      o << '[';
      for (size_t i = 0; i < sizeof (entry->mArray) / sizeof (entry->mArray[0]);
           i++)
        {
          if (entry->mArray[i])
            {
              if (flag)
                o << ',';
              o << (char)(isprint (i) ? i : '.') << ':' << entry->mArray[i];
              flag = true;
            }
        }
      o << ']';
    }
  o << std::endl;
}
           
void FuzzyPattern::insert (std::string const& s, int score)
{
  if (s.empty ())
    return;
  
  for (size_t i = 0; i < s.length (); i++)
    {
      insert (s[i], score);
    }
}

bool FuzzyPattern::compile (std::string const& pattern)
{
  char const *orig, *s;
  int state = kStateNormal;
  std::string a, b;

  reset ();

  if (pattern.empty ())
    {
      mError = kErrorEmptyPattern;
      mErrorPosition = 0;
      return false;
    }
  
  //
  // Use C-String for speed.
  //
  s = orig = pattern.c_str ();

  // Initialize
  mark ();
  
  while (*s)
    {
      switch (state)
        {
        case kStateNormal:
          switch (*s)
            {
            case ' ': case '\t': case '\r': case '\n':
                break;

            case '[':
                state = kStateScore;
                a.clear ();
                break;
/*
            case '{':
              ++s;
              
              b.clear ();
              while (isdigit (*s))
                b += *s++;
              
              if (b.empty ())
                {
                  mError = kErrorExpectDigits;
                  mErrorPosition = s - orig + 1;
                  return false;
                }
              
              if(*s != '}')
                {
                  mError = kErrorInvalidCharacter;
                  mErrorPosition = s - orig;
                  return false;
                }

			  repeat(::atoi (b.c_str ()));

              break;
*/
			case '%':
				mLabel = (s+1);
				s += strlen(s+1);
				break;

            default:
                if ( (*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z'))
                  {
                    insert (tolower (*s), 1);
                    mark ();
                    break;
                  }

                mError = kErrorInvalidCharacter;
                mErrorPosition = s - orig + 1;
                return false;
            }
          break;

        case kStateScore:
          switch (*s)
            {
            case ' ': case '\t': case '\r': case '\n':
                break;

            case ':':
              ++s;
              
              b.clear ();
              while (isdigit (*s))
                b += *s++;
              
              if (b.empty ())
                {
                  mError = kErrorExpectDigits;
                  mErrorPosition = s - orig + 1;
                  return false;
                }
              
              --s;

              insert (a, ::atoi (b.c_str ()));
              a.clear ();
              break;
              
            case ',':
              if (! a.empty ())
                {
                  insert (a, 1);
                }

              a.clear ();
              break;
              
            case ']':
              state = kStateNormal;
              if (! a.empty ())
                {
                  insert (a, 1);
                }
              mark ();
              break;

            default:
              if ( (*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z'))
                {
                  a += tolower (*s);
                }
            }
          break;

        default:
          mError = kErrorInvalidCharacter;
          mErrorPosition = s - orig + 1;
          return false;
        }

      s++;
    }

  if (state != kStateNormal)
    {
      mError = kErrorInvalidSequence;
      mErrorPosition = s - orig + 1;
      return false;
    }

  if (pack ())
    {
      mCompiled = true;
      mPattern = pattern;
    }

  return mCompiled;
}

bool FuzzyPattern::pack (void)
{
  PatternEntry *entry;
  size_t count, j;

  if (mSize)
    return true;
  
  if (!mFirst)
    return false;

  for (count = 0, entry = mFirst; entry != mLast; count++, entry = entry->mNext)
    ;

  resize (count);

  for (j = 0, entry = mFirst; entry != mLast; )
    {
      PatternEntry *next = entry->mNext;
      int sum = 0;

      for (size_t i = 0; i < kEntries; i++)
        {
          if (entry->mArray[i] > sum)
            sum = entry->mArray[i];
        }
      
      if (!sum)
        ++sum;
      
      for (size_t i = 0; i < kEntries; i++)
        {
          mArrays[j][i] = static_cast <REAL> (entry->mArray[i])
                        / static_cast <REAL> (sum);
        }
      
      j++;
      entry = next;
    }

  return true;
}

void FuzzyPattern::resize (size_t size)
{
  if (mArrays)
    {
      delete[] mArrays;
      mSize = 0;
    }

  if (size)
    {
      mArrays = new Array[size];
      mSize = size;
      memset (mArrays, 0, sizeof (Array) * size);
    }
}
