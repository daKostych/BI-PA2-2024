#pragma once

#include <iostream>
#include <set>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

class InputAnalyzer
{
public:
    InputAnalyzer() = default;
    InputAnalyzer(map<string, int> m);
    // analyze
    InputAnalyzer & analyze(istream & is);
    // count
    size_t count() const;
    //word
    size_t word(const string & word) const;
    // operator^
    friend InputAnalyzer operator^(const InputAnalyzer & first, const InputAnalyzer & second);

private:
    map<string, int> words;
};