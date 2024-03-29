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

struct Patch
{
    Patch & operator=(const Patch & old)
    {
        if (this == &old)
            return *this;

        offset = old.offset;
        patchLength = old.patchLength;
        strLengthIncludingPatch = old.strLengthIncludingPatch;
        patch = old.patch;
        return *this;
    };

    size_t offset = 0;
    size_t patchLength = 0;
    size_t strLengthIncludingPatch = 0;
    shared_ptr<char> patch = nullptr;
};

class CPatchStr
{
public:
    CPatchStr() = default;
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
    size_t patchBinarySearch(size_t from) const;
    void resizeString(size_t secSize);
    void stringInsert(const CPatchStr & src, size_t posToInsert);

    Patch * string;
    size_t size;
    size_t capacity;
    size_t totalLength;
};

/*
CPatchStr::CPatchStr() : size(0), capacity(1), totalLength(0)
{
    string = new Patch[capacity];
    char * emptyStr = new char[1]{'\0'};
    string[0].patch.reset(emptyStr);
};
*/

CPatchStr::CPatchStr(const char * str) : size(1),
                                         capacity(1),
                                         totalLength(strlen(str))
{
    string = new Patch[capacity];
    string[0].patchLength = strlen(str);
    string[0].strLengthIncludingPatch = strlen(str);

    char * tmpString = new char[strlen(str) + 1];
    strcpy(tmpString, str);
    string[0].patch.reset(tmpString);
}

CPatchStr::CPatchStr(const CPatchStr & str) : size(str.size),
                                              capacity(str.capacity),
                                              totalLength(str.totalLength)
{
    if(!str.totalLength)
    {
        size = 0, capacity = 1, totalLength = 0;
        string = new Patch[capacity];
        char * emptyStr = new char[1]{'\0'};
        string[0].patch.reset(emptyStr);
    }
    else
    {
        string = new Patch[str.capacity];
        for (size_t i = 0; i < str.size; i++)
            string[i] = str.string[i];
    }
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
    swap(totalLength, copyToSwap.totalLength);
    swap(string, copyToSwap.string);
    return *this;
}

char * CPatchStr::toStr() const
{
    char * copyToReturn = new char[totalLength + 1];
    for (size_t i = 0, shift = 0; i < size; i++)
    {
        memcpy(copyToReturn + shift, string[i].patch.get() + string[i].offset, string[i].patchLength);
        shift += string[i].patchLength;
    }
    return copyToReturn;
}

CPatchStr CPatchStr::subStr(size_t from, size_t len) const
{
    if (from + len > totalLength)
        throw out_of_range("Out of range");

    size_t lastSymbol = len;
    size_t start = patchBinarySearch(from);
    char * subString = new char[len + 1];
    for (size_t i = start, shift = 0; len; i++)
    {
        if (string[i].patchLength == 0)
            continue;

        /*cout << "patch:                   " << string[i].patch.get() << endl
             << "offset:                  " << string[i].offset << endl
             << "patchLength:             " << string[i].patchLength << endl
             << "strLengthIncludingPatch: " << string[i].strLengthIncludingPatch << endl
             << "----------------------------------------------------------" << endl;*/

        size_t copyLen;
        if (i == start)
        {
            copyLen = len < (string[i].strLengthIncludingPatch - from) ? len
                                                                       : string[i].strLengthIncludingPatch - from;
            memcpy(subString + shift,
                   string[i].patch.get() + string[i].offset + from - (string[i].strLengthIncludingPatch - string[i].patchLength),
                   copyLen);
            shift += copyLen;
            len -= copyLen;
        }
        else
        {
            copyLen = len < string[i].patchLength ? len
                                                  : string[i].patchLength;
            memcpy(subString + shift,
                   string[i].patch.get() + string[i].offset,
                   copyLen);
            shift += copyLen;
            len -= copyLen;
        }
    }
    subString[lastSymbol] = '\0';

    CPatchStr toReturn(subString);
    delete [] subString;
    return toReturn;
}

CPatchStr &CPatchStr::append(const CPatchStr &src)
{
    if (!src.string)
        return *this;

    if (capacity < src.size + size)
        this->resizeString(src.size);

    for(size_t i = 0; i < src.size; i++)
    {
        string[size + i] = src.string[i];
        size + i ? string[size + i].strLengthIncludingPatch = string[size + i - 1].strLengthIncludingPatch + string[size + i].patchLength
                 : string[size + i].strLengthIncludingPatch = string[size + i].patchLength;
    }

    size += src.size;
    totalLength += src.totalLength;
    return *this;
}


CPatchStr &CPatchStr::insert(size_t pos, const CPatchStr &src)
{

    if (pos > size)
        throw out_of_range("Out of range");
    else if (!src.string)
        return *this;
    else if (pos == totalLength + 1)
        this->append(src);
    else
    {
        if (capacity < src.size + size)
            this->resizeString(src.size);

        size_t posToInsert = patchBinarySearch(pos);

        if (pos == string[posToInsert].strLengthIncludingPatch - string[posToInsert].patchLength)
            this->stringInsert(src, posToInsert);
        else
        {
            char * splitString = new char[string[posToInsert].strLengthIncludingPatch - pos + 1];
            memcpy(splitString,
                   string[posToInsert].patch.get()
                        + string[posToInsert].offset
                        + (pos - (string[posToInsert].strLengthIncludingPatch - string[posToInsert].patchLength)),
                   string[posToInsert].strLengthIncludingPatch - pos);

            splitString[string[posToInsert].strLengthIncludingPatch - pos] = '\0';

            string[posToInsert].patchLength -= (string[posToInsert].strLengthIncludingPatch - pos);
            string[posToInsert].strLengthIncludingPatch -= (string[posToInsert].strLengthIncludingPatch - pos);

            this->stringInsert(CPatchStr(splitString), posToInsert + 1);
            if (capacity < src.size + size)
                this->resizeString(src.size);

            this->stringInsert(src, posToInsert + 1);

            delete [] splitString;
        }
    }

    return *this;
}

CPatchStr &CPatchStr::remove(size_t from, size_t len)
{
    /*
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
    */
    return *this;
};


void CPatchStr::resizeString(size_t secSize)
{
    capacity = pow((secSize + size), 2) + 1; // maybe mistake
    auto * newString = new Patch[capacity];

    for (size_t i = 0; i < size; i++)
        newString[i] = string[i];

    delete [] string;
    string = newString;
};

size_t CPatchStr::patchBinarySearch(const size_t from) const
{
    size_t upper = size - 1,
           lower = 0,
           pos = upper - (upper - lower) / 2;

    /*cout << "patch:                   " << string[pos].patch.get() << endl
         << "offset:                  " << string[pos].offset << endl
         << "patchLength:             " << string[pos].patchLength << endl
         << "strLengthIncludingPatch: " << string[pos].strLengthIncludingPatch << endl
         << "----------------------------------------------------------" << endl;*/

    while (! ((string[pos].strLengthIncludingPatch - string[pos].patchLength <= from)
               && (from <= string[pos].strLengthIncludingPatch)))
    {
        if (from < string[pos].strLengthIncludingPatch)
            upper = pos - 1;
        else
            lower = pos + 1;
        pos = upper - (upper - lower) / 2;

        /*cout << "patch:                   " << string[pos].patch.get() << endl
             << "offset:                  " << string[pos].offset << endl
             << "patchLength:             " << string[pos].patchLength << endl
             << "strLengthIncludingPatch: " << string[pos].strLengthIncludingPatch << endl
             << "----------------------------------------------------------" << endl;*/
    }
    return pos;
}

void CPatchStr::stringInsert(const CPatchStr &src, size_t posToInsert)
{
    for (size_t i = size - 1; i >= posToInsert; i--)
    {
        /*cout << "patch:                   " << string[i].patch.get() << endl
             << "offset:                  " << string[i].offset << endl
             << "patchLength:             " << string[i].patchLength << endl
             << "strLengthIncludingPatch: " << string[i].strLengthIncludingPatch << endl
             << "----------------------------------------------------------" << endl;*/

        string[i + src.size] = string[i];
        string[i + src.size].strLengthIncludingPatch += src.totalLength;

        /*cout << "patch:                   " << string[i].patch.get() << endl
             << "offset:                  " << string[i].offset << endl
             << "patchLength:             " << string[i].patchLength << endl
             << "strLengthIncludingPatch: " << string[i].strLengthIncludingPatch << endl
             << "----------------------------------------------------------" << endl;*/
    }
    for (size_t i = 0; i < src.size; i++)
    {
        /*cout << "patch:                   " << string[posToInsert + i].patch.get() << endl
             << "offset:                  " << string[posToInsert + i].offset << endl
             << "patchLength:             " << string[posToInsert + i].patchLength << endl
             << "strLengthIncludingPatch: " << string[posToInsert + i].strLengthIncludingPatch << endl
             << "----------------------------------------------------------" << endl;*/

        string[posToInsert + i] = src.string[i];
        posToInsert + i ? string[posToInsert + i].strLengthIncludingPatch = string[posToInsert + i - 1].strLengthIncludingPatch
                                                                            + string[posToInsert + i].patchLength
                        : string[posToInsert + i].strLengthIncludingPatch = string[posToInsert + i].patchLength;

        /*cout << "patch:                   " << string[posToInsert + i].patch.get() << endl
             << "offset:                  " << string[posToInsert + i].offset << endl
             << "patchLength:             " << string[posToInsert + i].patchLength << endl
             << "strLengthIncludingPatch: " << string[posToInsert + i].strLengthIncludingPatch << endl
             << "----------------------------------------------------------" << endl;*/
    }
    size += src.size;
    totalLength += src.totalLength;
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
