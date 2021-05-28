#ifndef __ShiftRegister_h
#define __ShiftRegister_h

#include <vector>

template <typename T>
class ShiftRegister
  {
    T *mBuffer;
    size_t mSize;
    ssize_t mPosition;

public:

// Constructors
    // An empty guy
    ShiftRegister (size_t const size = 0)
      {
        build(size);
      }

    // Prefill with a value
    ShiftRegister (size_t const size, T const& init)
      {
        build(size);
        for (size_t i = 0; i < size; i++)
          mBuffer[i] = init;
      }

    // Prefill with anything resembling a vector having some size()
    template <typename V>
    ShiftRegister (V const &vec)
      {
        build(vec.size());
        for (size_t i = 0; i < vec.size(); i++)
          mBuffer[i] = vec[i];
      }

    // Prefill with anything resembling an array, with a given size
    template <typename A>
    ShiftRegister (size_t const asize, A const &arr)
      {
        build(asize);
        for (size_t i = 0; i < asize; i++)
          mBuffer[i] = arr[i];
      }

    // Prefill with anything resembling an array, with a given asize,
    // but then resize the thing to rsize
    template <typename A>
    ShiftRegister (size_t const asize, size_t const rsize, A const &arr)
      {
        build(rsize);
        for (size_t i = 0; i < rsize; i++)
          mBuffer[i] = arr[i % asize];
      }

    // Destructor
    ~ShiftRegister (void)
      {
        if (!empty())
          delete[] mBuffer;
      }

    // Methods available in vector
    inline size_t size (void) const
      {
        return mSize;
      }

    inline bool empty (void) const
      {
        return (size()==0);
      }

    inline void resize (size_t const size)
      {
        if (size == mSize)
          return;

        if (size)
          {
            T *tmp = new T[size];

            for (size_t i = 0; i < size; i++)
              tmp[i] = at(i);

            delete mBuffer;
            mBuffer = tmp;

            mSize = size;
            mPosition = 0;
          }
        else
          {
            mBuffer = NULL;
            mSize = 0;
            mPosition = 0;
          }
      }

    // Does range check and wraps properly
    inline T& at (ssize_t const index) const
      {
        ssize_t i = index + static_cast<ssize_t> (mPosition);
        while(i < 0)
          i += static_cast<ssize_t> (size());
        while(i >= (ssize_t) size())
          i -= static_cast<ssize_t> (size());
        return mBuffer[i];
      }
    
    inline T& front (void) const
      {
        return at (0);
      }

    inline T& back (void) const
      {
        return at(size());
      }

    void clear(void)
      {
        resize(0);
      }

    // No check: must be from -size to (size-1) !
    inline T& operator [] (ssize_t position) const
      {
        ssize_t i = position + static_cast<ssize_t> (mPosition);
        if(position > 0)
          {
            if (i >= (ssize_t) size())
              i -= size();
          }
        else
          {
            if (i < 0)
              i += size();
          }
        return mBuffer[i];
      }

#ifdef UNSIGNED_HACK
    // Uhhh even more optimized, we know it's unsigned !
    inline T& operator [] (size_t position) const
      {
        ssize_t i = position + static_cast<ssize_t> (mPosition);
        if (i >= size())
          i -= size();
        return mBuffer[i];
      }
#endif

    /*    XXX Feel free to write this, the comparison operator and an iterator :) */
    /*    swap(ShiftRegister<T>) */

// Our specific methods
    // Fill it all with a given value
    void fill(T const& init)
      {
        for (size_t i = 0; i < mSize; i++)
          mBuffer[i] = init;
        mPosition = 0;
    }

    // Shift all down one position, inserting a new value at the top and
    // returning what was at [0]
    inline T const & shift_down (T const & value)
      {
        if (!empty())
          {
            T& tmp = mBuffer[mPosition];
            mBuffer[mPosition++] = value;
            if (mPosition == (ssize_t) size())
              mPosition = 0;
            return tmp;
          }
        return value;
      }

    // Shifts all up one position, puts what gets as argument in [0]
    // and returns what was in top()
    inline T& shift_up (T value)
      {
        if (!empty())
          {
            if (!mPosition)
              mPosition = size();
            T& tmp = mBuffer[--mPosition];
            mBuffer[mPosition] = value;
            return tmp;
          }
        return value;
      }

    inline void rot (ssize_t n = 1)
      {
        ssize_t i = n + static_cast<ssize_t> (mPosition);
        while(i < 0)
          i += static_cast<ssize_t> (size());
        while(i >= size())
          i -= static_cast<ssize_t> (size());
        mPosition = i;
      }

#ifndef TEST_SHIFTREGISTER
private:
#endif

    // Lame, lame, LAME c++: why can't a constructor call another constructor ?
    void build(size_t size)
    {
      mSize = size;
      mPosition = 0;
      if(size)
        mBuffer = new T[size];
      else
        mBuffer = NULL;
    }

    // Maybe some day it will be used, by now I just like it,
    // realigns the array so that mPosition becomes 0 and nothing
    // else changes. Linear in time, works in place.
    void realign(void)
      {
        size_t todo;
        size_t block = mPosition;
        size_t done = 0;
        size_t rest = (size() - mPosition);
        while ( (todo = (block < rest) ? block : rest) )
          {
            for (size_t i = 0; i < todo; i++, done++)
              {
                T tmp = mBuffer[done];
                mBuffer[done] = mBuffer[done+block];
                mBuffer[done+block] = tmp;
              }

            if (block > rest)
              block -= todo;
            else
              rest -= todo;
          }
        mPosition = 0;
      }

  };

#endif  // !__ShiftRegister_h
