#include "InputAnalyzer.hpp"

#include <utility>

InputAnalyzer::InputAnalyzer(map<string, int> m)
{
    words = std::move(m);
}

InputAnalyzer & InputAnalyzer::analyze(istream & is)
{
    string word;
    while (is >> word)
        words[word]++;
    return *this;
};

size_t InputAnalyzer::count() const { return words.size(); };

size_t InputAnalyzer::word(const string & word) const
{
    if (words.count(word) == 0)
        return 0;
    else
        return words.at(word);
};

InputAnalyzer operator^(const InputAnalyzer & first, const InputAnalyzer & second)
{
    map<string, int> toReturn;
    auto lambda = [&](const pair<string, int> & lhs, const pair<string, int> & rhs)
    {
        return lhs.first < rhs.first;
    };
    set_symmetric_difference(first.words.begin(), first.words.end(),
                             second.words.begin(), second.words.end(),
                             inserter(toReturn, toReturn.begin()),
                             lambda);
    return {toReturn};
};