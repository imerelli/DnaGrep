#ifndef __SequenceInfo_h
#define __SequenceInfo_h

#include <vector>
#include <string>

class SequenceInfo
  {
    std::string mSequence;
    std::string mName;
    std::string mComment;

public:

// Constructors

    // Prefill with a string as content
    SequenceInfo (std::string const& init = "", std::string const& name = "", std::string const& comment = "")
    : mSequence(init),
      mName(name),
      mComment(comment)
      {
      }

    // Destructor
    ~SequenceInfo (void)
      {
      }

    // Methods available in vector
    inline size_t size (void) const
      {
        return mSequence.size();
      }

    inline bool empty (void) const
      {
        return mSequence.empty();
      }

    inline void resize (size_t const size)
      {
        return mSequence.resize(size);
      }

    void clear(void)
      {
        resize(0);
      }

    inline char const operator [] (ssize_t position) const
      {
        if (position < 0 || position > (ssize_t) size())
          return '-';
        return  mSequence[position];
      }

    inline void setSequencePos(size_t position, char ch)
      {
        if (!(position > size()))
          mSequence[position] = ch;
      }
      
    inline std::string Comment(void)
      {
        return mComment;
      }

    inline std::string Name(void)
      {
        return mName;
      }

  };

#endif  // !__SequenceInfo_h
