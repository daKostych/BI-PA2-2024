#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#endif /* __PROGTEST__ */

using namespace std;

class CPatchStr
{
  public:
    CPatchStr();
    CPatchStr(const char * str);
    // copy constructor
    CPatchStr(const CPatchStr & str);
    // destructor
    ~CPatchStr();
    // operator =
    CPatchStr & operator=(const CPatchStr & newStr);
    CPatchStr subStr(size_t from, size_t len) const;
    CPatchStr & append(const CPatchStr & src);

    CPatchStr & insert(size_t pos, const CPatchStr & src);
    CPatchStr & remove(size_t from, size_t len);
    char * toStr() const;

  // private:
    void resizeString(size_t secSize);

    char * string = nullptr;
    size_t size;
    size_t capacity;
};

CPatchStr::CPatchStr() : size(0), capacity(1)
{
    string = new char[capacity];
    string[0] = '\0';
};

CPatchStr::CPatchStr(const char * str) : size(strlen(str)), capacity(strlen(str) + 1)
{
    string = new char[capacity];
    strcpy(string, str);
}

CPatchStr::CPatchStr(const CPatchStr & str)
{
    string = new char[strlen(str.string) + 1];
    strcpy(string, str.string);
    size = str.size;
    capacity = str.capacity;
};

CPatchStr::~CPatchStr()
{
    delete [] string;
    string = nullptr;
    size = 0;
    capacity = 0;
}

CPatchStr & CPatchStr::operator=(const CPatchStr & newStr)
{
    if (this == &newStr)
        return *this;

    CPatchStr copyToSwap(newStr);
    swap(size, copyToSwap.size);
    swap(capacity, copyToSwap.capacity);
    swap(string, copyToSwap.string);
    return *this;
}

char * CPatchStr::toStr() const
{
    char * copyToReturn = new char[size+1];
    memcpy(copyToReturn, string, size + 1);
    return copyToReturn;
}

CPatchStr CPatchStr::subStr(size_t from, size_t len) const
{
    if (from + len > size)
        throw out_of_range("Out of range");

    char * subString = new char[len + 1];
    memcpy(subString, string + from, len);
    subString[len] = '\0';

    CPatchStr toReturn(subString);
    delete [] subString;
    return toReturn;
}

CPatchStr &CPatchStr::append(const CPatchStr &src)
{
    if (!src.size)
        return *this;

    if (capacity < src.size + size + 1)
        this->resizeString(src.size);
    if (this != &src)
        memcpy(string + size, src.string, src.size + 1);
    else
    {
        char * copySrc = new char[src.size + 1];
        memcpy(copySrc, src.string, src.size + 1);
        strcat(string + size, copySrc);
        delete [] copySrc;
    }
    size += src.size;
    return *this;
}

CPatchStr &CPatchStr::insert(size_t pos, const CPatchStr &src)
{
    if (pos > size)
        throw out_of_range("Out of range");
    else if (!src.size)
        return *this;
    else if (pos == size)
        this->append(src);
    else
    {
        if (capacity < src.size + size + 1)
            this->resizeString(src.size);
        char * partToShift = new char[size + 1 - pos];
        memcpy(partToShift, string + pos, size + 1 - pos);
        memcpy(string + pos, src.string, src.size);
        memcpy(string + pos + src.size, partToShift, size + 1 - pos);
        delete [] partToShift;
        size += src.size;
    }
    return *this;
}

CPatchStr &CPatchStr::remove(size_t from, size_t len)
{
    if (from + len > size)
        throw out_of_range("Out of range");
    else if (!len)
        return *this;
    else
    {
        char * partToShift = new char[size + 1 - from - len];
        memcpy(partToShift, string + from + len, size + 1 - from - len);
        memcpy(string + from, partToShift, size + 1 - from - len);
        delete [] partToShift;
        size -= len;
    }
    return *this;
};

void CPatchStr::resizeString(size_t secSize)
{
    capacity = secSize + size + 1;
    char * newString = new char[capacity];
    memcpy(newString, string, size + 1);
    delete [] string;
    string = newString;
};

#ifndef __PROGTEST__
bool stringMatch ( char       * str,
                   const char * expected )
{
  bool res = std::strcmp ( str, expected ) == 0;
  delete [] str;
  return res;
}

int main ()
{
  char tmpStr[100];

  CPatchStr a ( "test" );
  assert ( stringMatch ( a . toStr (), "test" ) );
  std::strncpy ( tmpStr, " da", sizeof ( tmpStr ) - 1 );
  a . append ( tmpStr );
  assert ( stringMatch ( a . toStr (), "test da" ) );
  std::strncpy ( tmpStr, "ta", sizeof ( tmpStr ) - 1 );
  a . append ( tmpStr );
  assert ( stringMatch ( a . toStr (), "test data" ) );
  std::strncpy ( tmpStr, "foo text", sizeof ( tmpStr ) - 1 );
  CPatchStr b ( tmpStr );
  assert ( stringMatch ( b . toStr (), "foo text" ) );
  CPatchStr c ( a );
  assert ( stringMatch ( c . toStr (), "test data" ) );
  CPatchStr d ( a . subStr ( 3, 5 ) );
  assert ( stringMatch ( d . toStr (), "t dat" ) );
  d . append ( b );
  assert ( stringMatch ( d . toStr (), "t datfoo text" ) );
  d . append ( b . subStr ( 3, 4 ) );
  assert ( stringMatch ( d . toStr (), "t datfoo text tex" ) );
  c . append ( d );
  assert ( stringMatch ( c . toStr (), "test datat datfoo text tex" ) );
  c . append ( c );
  assert ( stringMatch ( c . toStr (), "test datat datfoo text textest datat datfoo text tex" ) );
  d . insert ( 2, c . subStr ( 6, 9 ) );
  assert ( stringMatch ( d . toStr (), "t atat datfdatfoo text tex" ) );
  b = "abcdefgh";
  assert ( stringMatch ( b . toStr (), "abcdefgh" ) );
  assert ( stringMatch ( d . toStr (), "t atat datfdatfoo text tex" ) );
  assert ( stringMatch ( d . subStr ( 4, 8 ) . toStr (), "at datfd" ) );
  assert ( stringMatch ( b . subStr ( 2, 6 ) . toStr (), "cdefgh" ) );
  try
  {
    b . subStr ( 2, 7 ) . toStr ();
    assert ( "Exception not thrown" == nullptr );
  }
  catch ( const std::out_of_range & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown" == nullptr );
  }
  a . remove ( 3, 5 );
  assert ( stringMatch ( a . toStr (), "tesa" ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
