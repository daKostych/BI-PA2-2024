#ifndef __PROGTEST__

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <array>
#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <compare>
#include <algorithm>
#include <cassert>
#include <memory>
#include <iterator>
#include <functional>
#include <stdexcept>

using namespace std::literals;

class CDummy
{
public:
    CDummy(char c) : m_C(c)
    {
    }

    bool operator==(const CDummy & other) const = default;

    friend std::ostream & operator<<(std::ostream & os, const CDummy & x)
    {
        return os << '\'' << x.m_C << '\'';
    }

private:
    char m_C;
};

#endif /* __PROGTEST__ */

// #define TEST_EXTRA_INTERFACE

using namespace std;

template<typename T_>
class CSelfMatch
{
public:
    struct SubSequence
    {
        template<typename I_>
        SubSequence(I_ begin, I_ end, size_t index)
        {
            _subSequence = vector<T_>(begin, end);
            _indexes.push_back(index);
        }

        bool operator<(const SubSequence & rhs) const { return _subSequence.size() > rhs._subSequence.size(); };
        friend bool operator==(const SubSequence & lhs, const SubSequence & rhs)
        {
            return lhs._subSequence == rhs._subSequence;
        };

        vector<T_> _subSequence;
        size_t _appearanceCnt = 1;
        vector<size_t> _indexes;
    };
    // constructor (initializer list)
    CSelfMatch(initializer_list<T_> initList) { _sequence = initList; }
    // constructor (2 iterators)
    template<typename I_>
    CSelfMatch(I_ begin, I_ end) { _sequence = vector<T_>(begin, end); }
    // constructor (a container)
    template<typename C_>
    CSelfMatch(C_ container) { _sequence = vector<T_>(container.begin(), container.end()); }

    // optionally: push_back
//======================================================================================================================
    size_t sequenceLen ( size_t n ) const
    {
        if (!n || n > _sequence.size())
            throw invalid_argument("N must be in the range: 0 < N < size of container!");
        if (n == 1)
            return _sequence.size();

        vector<SubSequence> subSequences;
        analyzeSubSequences(subSequences);

        for (const auto & subSeq : subSequences)
            if (subSeq._appearanceCnt >= n)
                return subSeq._subSequence.size();
        return 0;
    }
//======================================================================================================================
    template<size_t N_>
    std::vector<std::array<size_t, N_>> findSequences () const
    {
        if (!N_ || N_ > _sequence.size())
            throw invalid_argument("N must be in the range: 0 < N < size of container!");
        if (N_ == 1)
            return {{0}};

        vector<SubSequence> subSequences;
        analyzeSubSequences(subSequences);

        vector<array<size_t, N_>> result;
        size_t max = 0;
        for (const auto & subSeq : subSequences)
        {
            if (subSeq._appearanceCnt >= N_ && subSeq._subSequence.size() >= max)
            {
                max = subSeq._subSequence.size();
                array<size_t, N_> tmp{};
                addCombs(subSeq._indexes, result, tmp, 0, N_, 0);
            }
        }
        return result;
    }

private:
//======================================================================================================================
    template<size_t N_>
    void addCombs(const vector<size_t> & indexes,
                  vector<array<size_t, N_>> & result,
                  array<size_t, N_> & tmpArray,
                  size_t index, size_t N, size_t posInArray) const
    {
        if (N == 0)
        {
            result.push_back(tmpArray);
            return;
        }
        for (size_t i = index; i < indexes.size(); i++)
        {
            tmpArray[posInArray] = indexes[i];
            addCombs(indexes, result, tmpArray, i + 1, N - 1, posInArray + 1);
        }
    }
//======================================================================================================================
    void analyzeSubSequences(vector<SubSequence> & subSequences) const
    {
        for (size_t i = 0; i < _sequence.size(); i++)
        {
            for (size_t j = i + 1; j < _sequence.size() + 1; j++)
            {
                SubSequence subSequence(_sequence.begin() + i, _sequence.begin() + j, i);
                auto pos1 = lower_bound(subSequences.begin(), subSequences.end(),
                                        subSequence);
                auto pos2 = upper_bound(subSequences.begin(), subSequences.end(),
                                        subSequence);
                if (pos1 != subSequences.end())
                {
                    bool found = false;
                    for (auto pos = pos1; pos != pos2; pos++)
                        if (*pos == subSequence)
                        {
                            (*pos)._appearanceCnt++;
                            (*pos)._indexes.push_back(i);
                            found = true;
                            break;
                        }
                    if (!found)
                        subSequences.insert(pos1, SubSequence(subSequence));
                }
                else
                    subSequences.emplace_back(subSequence);
            }
        }
    }
//======================================================================================================================
    vector<T_> _sequence;
};


#ifndef __PROGTEST__

template<size_t N_>
bool positionMatch(std::vector<std::array<size_t, N_>> a, std::vector<std::array<size_t, N_>> b)
{
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a == b;
}

int main()
{
    CSelfMatch<char> x0("aaaaaaaaaaa"s);
    assert (x0.sequenceLen(2) == 10);
    assert (positionMatch(x0.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 1}}));
    CSelfMatch<char> x1("abababababa"s);
    assert (x1.sequenceLen(2) == 9);
    assert (positionMatch(x1.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 2}}));
    CSelfMatch<char> x2("abababababab"s);
    assert (x2.sequenceLen(2) == 10);
    assert (positionMatch(x2.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 2}}));
    CSelfMatch<char> x3("aaaaaaaaaaa"s);
    assert (x3.sequenceLen(3) == 9);
    assert (positionMatch(x3.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 1, 2}}));
    CSelfMatch<char> x4("abababababa"s);
    assert (x4.sequenceLen(3) == 7);
    assert (positionMatch(x4.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 2, 4}}));
    CSelfMatch<char> x5("abababababab"s);
    assert (x5.sequenceLen(3) == 8);
    assert (positionMatch(x5.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 2, 4}}));
    CSelfMatch<char> x6("abcdXabcd"s);
    assert (x6.sequenceLen(1) == 9);
    assert (positionMatch(x6.findSequences<1>(), std::vector<std::array<size_t, 1> >{{0}}));
    CSelfMatch<char> x7("abcdXabcd"s);
    assert (x7.sequenceLen(2) == 4);
    assert (positionMatch(x7.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 5}}));
    CSelfMatch<char> x8("abcdXabcdeYabcdZabcd"s);
    assert (x8.sequenceLen(2) == 4);
    assert (positionMatch(x8.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0,  5},
                                                                                     {0,  11},
                                                                                     {0,  16},
                                                                                     {5,  11},
                                                                                     {5,  16},
                                                                                     {11, 16}}));
    CSelfMatch<char> x9("abcdXabcdYabcd"s);
    assert (x9.sequenceLen(3) == 4);
    assert (positionMatch(x9.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 5, 10}}));
    CSelfMatch<char> x10("abcdefghijklmn"s);
    assert (x10.sequenceLen(2) == 0);
    assert (positionMatch(x10.findSequences<2>(), std::vector<std::array<size_t, 2> >{}));
    CSelfMatch<char> x11("abcXabcYabcZdefXdef"s);
    assert (x11.sequenceLen(2) == 3);
    assert (positionMatch(x11.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0,  4},
                                                                                      {0,  8},
                                                                                      {4,  8},
                                                                                      {12, 16}}));
    CSelfMatch<int> x12{1, 2, 3, 1, 2, 4, 1, 2};
    assert (x12.sequenceLen(2) == 2);
    assert (positionMatch(x12.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 3},
                                                                                      {0, 6},
                                                                                      {3, 6}}));
    assert (x12.sequenceLen(3) == 2);
    assert (positionMatch(x12.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 3, 6}}));
    std::initializer_list<CDummy> init13{'a', 'b', 'c', 'd', 'X', 'a', 'b', 'c', 'd', 'Y', 'a', 'b', 'c', 'd'};
    CSelfMatch<CDummy> x13(init13.begin(), init13.end());
    assert (x13.sequenceLen(2) == 4);
    assert (positionMatch(x13.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 5},
                                                                                      {0, 10},
                                                                                      {5, 10}}));
    std::initializer_list<int> init14{1, 2, 1, 1, 2, 1, 0, 0, 1, 2, 1, 0, 1, 2, 0, 1, 2, 0, 1, 1, 1, 2, 0, 2, 0, 1, 2,
                                      1, 0};
    CSelfMatch<int> x14(init14.begin(), init14.end());
    assert (x14.sequenceLen(2) == 5);
    assert (positionMatch(x14.findSequences<2>(), std::vector<std::array<size_t, 2> >{{11, 14},
                                                                                      {7,  24}}));
    std::initializer_list<int> init15{1, 2, 1, 1, 2, 1, 0, 0, 1, 2, 1, 0, 1, 2, 0, 1, 2, 0, 1, 1, 1, 2, 0, 2, 0, 1, 2,
                                      1, 0};
    CSelfMatch<int> x15(init15.begin(), init15.end());
    assert (x15.sequenceLen(3) == 4);
    assert (positionMatch(x15.findSequences<3>(), std::vector<std::array<size_t, 3> >{{3, 8, 25}}));
#ifdef TEST_EXTRA_INTERFACE
    CSelfMatch y0 ( "aaaaaaaaaaa"s );
    assert ( y0 . sequenceLen ( 2 ) == 10 );

    std::string s1 ( "abcd" );
    CSelfMatch y1 ( s1 . begin (), s1 . end () );
    assert ( y1 . sequenceLen ( 2 ) == 0 );

    CSelfMatch y2 ( ""s );
    y2 . push_back ( 'a', 'b', 'c', 'X' );
    y2 . push_back ( 'a' );
    y2 . push_back ( 'b', 'c' );
    assert ( y2 . sequenceLen ( 2 ) == 3 );
#endif /* TEST_EXTRA_INTERFACE */
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
