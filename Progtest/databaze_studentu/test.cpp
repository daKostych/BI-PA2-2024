#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <utility>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <compare>

class CDate
{
public:
    CDate(int y, int m, int d) : m_Y(y), m_M(m), m_D(d) {};

    std::strong_ordering operator<=>(const CDate & other) const = default;

    friend std::ostream & operator<<(std::ostream & os, const CDate & d)
    {
        return os << d.m_Y << '-' << d.m_M << '-' << d.m_D;
    }

private:
    int m_Y;
    int m_M;
    int m_D;
};

enum class ESortKey
{
    NAME, BIRTH_DATE, ENROLL_YEAR
};

#endif /* __PROGTEST__ */

using namespace std;

string toLow(const string & s)
{
    string low = s;
    for (char & c : low)
        c = tolower(c);
    return low;
}

void nameToSet(set<string> & s, const string & name)
{
    istringstream iName(name);
    string namePart;
    while (iName >> namePart)
        s.insert(toLow(namePart));
}

multiset<string> nameToMultiset(const string & name)
{
    istringstream iName(toLow(name));
    multiset<string> nameParts;
    string namePart;

    while (iName >> namePart)
        nameParts.insert(namePart);

    return nameParts;
}

//----------------------------------------------------------------------------------------------------------------------
class CStudent
{
public:
    CStudent(const std::string & name, const CDate & born, int enrolled);
    explicit CStudent(const multiset<string> & name);
    explicit CStudent(const CDate & born);
    explicit CStudent(int enrolled);
    CStudent(const CStudent & s, int id);

    bool operator<(const CStudent & other) const;
    bool operator==(const CStudent & other) const;
    bool operator!=(const CStudent & other) const;

//private:
    multiset<string> m_Name;
    string m_StringName;
    CDate m_BirthDate = CDate(-1, -1, -1);
    int m_EnrollYear = -1;
    int m_HistoryId = -1;
};

//---------------------------------------------------------------------------
CStudent::CStudent(const string & name, const CDate & born, int enrolled) : m_BirthDate(born), m_EnrollYear(enrolled)
{
    m_Name = nameToMultiset(name);
    m_StringName = name;
}

//---------------------------------------------------------------------------
CStudent::CStudent(const multiset<string> & name) : m_Name(name) {}

//---------------------------------------------------------------------------
CStudent::CStudent(const CDate & born) : m_BirthDate(born) {}

//---------------------------------------------------------------------------
CStudent::CStudent(int enrolled) : m_EnrollYear(enrolled) {}

//---------------------------------------------------------------------------
CStudent::CStudent(const CStudent & s, int id) : m_Name(s.m_Name),
                                                 m_StringName(s.m_StringName),
                                                 m_BirthDate(s.m_BirthDate),
                                                 m_EnrollYear(s.m_EnrollYear),
                                                 m_HistoryId(id) {}

//---------------------------------------------------------------------------
bool CStudent::operator<(const CStudent & other) const
{
    if (m_EnrollYear != other.m_EnrollYear)
        return m_EnrollYear < other.m_EnrollYear;
    else
        return m_HistoryId < other.m_HistoryId;
}

//---------------------------------------------------------------------------
bool CStudent::operator==(const CStudent & other) const
{
    if (m_StringName == other.m_StringName &&
        m_BirthDate == other.m_BirthDate &&
        m_EnrollYear == other.m_EnrollYear)
        return true;
    return false;
}

bool CStudent::operator!=(const CStudent & other) const { return !(*this == other); }

//----------------------------------------------------------------------------------------------------------------------
class CFilter
{
public:
    CFilter();

    CFilter & name(const std::string & name);
    CFilter & bornBefore(const CDate & date);
    CFilter & bornAfter(const CDate & date);
    CFilter & enrolledBefore(int year);
    CFilter & enrolledAfter(int year);
    bool nameMatch(const multiset<string> & studentName) const;
    bool birthMatch(const CDate & date) const;
    bool validCheck() const;

//private:
    bool m_Valid = true;
    int m_EnrolledBefore;
    int m_EnrolledAfter;
    CDate m_BornBefore = CDate(INT_MAX, INT_MAX, INT_MAX);
    CDate m_BornAfter = CDate(-1, -1, -1);
    vector<multiset<string>> m_Names;
};

//---------------------------------------------------------------------------
CFilter::CFilter() : m_EnrolledBefore(INT_MAX), m_EnrolledAfter(-1) {}

//---------------------------------------------------------------------------
CFilter & CFilter::name(const string & name)
{
    multiset<string> n = nameToMultiset(toLow(name));
    m_Names.push_back(n);
    return *this;
}

//---------------------------------------------------------------------------
CFilter & CFilter::bornBefore(const CDate & date)
{
    m_BornBefore = date;
    m_Valid = validCheck();
    return *this;
}

//---------------------------------------------------------------------------
CFilter & CFilter::bornAfter(const CDate & date)
{
    m_BornAfter = date;
    m_Valid = validCheck();
    return *this;
}

//---------------------------------------------------------------------------
CFilter & CFilter::enrolledBefore(int year)
{
    m_EnrolledBefore = year;
    m_Valid = validCheck();
    return *this;
}

//---------------------------------------------------------------------------
CFilter & CFilter::enrolledAfter(int year)
{
    m_EnrolledAfter = year;
    m_Valid = validCheck();
    return *this;
}

//---------------------------------------------------------------------------
bool CFilter::validCheck() const
{
    if (m_EnrolledBefore <= m_EnrolledAfter ||
        m_BornAfter == m_BornBefore ||
        m_BornAfter >= m_BornBefore)
        return false;
    if (m_EnrolledBefore - m_EnrolledAfter == 1)
        return false;
    return true;
}

//---------------------------------------------------------------------------
bool CFilter::nameMatch(const multiset<string> & studentName) const
{
    bool suitName = false;
    if (m_Names.empty())
        suitName = true;
    else
    {
        for (auto & name : m_Names)
            if (name == studentName)
                suitName = true;
    }

    return suitName;
}

bool CFilter::birthMatch(const CDate & date) const
{
    if (date >= m_BornBefore ||
        date <= m_BornAfter)
        return false;

    return true;
}

//----------------------------------------------------------------------------------------------------------------------
class CSort
{
public:
    CSort() = default;

    CSort & addKey(ESortKey key, bool ascending);

//private:
    vector<pair<ESortKey, bool>> sortKeys;
};

//---------------------------------------------------------------------------
CSort & CSort::addKey(ESortKey key, bool ascending)
{
    sortKeys.emplace_back(key, ascending);
    return *this;
}

//----------------------------------------------------------------------------------------------------------------------
class CStudyDept
{
public:
    CStudyDept() = default;

    bool addStudent(const CStudent & x);
    bool delStudent(const CStudent & x);
    std::list<CStudent> search(const CFilter & flt, const CSort & sortOpt) const;
    std::set<std::string> suggest(const std::string & name) const;

private:
    static bool compareStudentsByBirthDate(const CStudent & s1, const CStudent & s2);
    static bool compareStudentsByEnroll(const CStudent & s1, const CStudent & s2);
    struct compareByEnroll
    {
        bool operator()(const CStudent & s1, const CStudent & s2) const
        {
            return compareStudentsByEnroll(s1, s2);
        }
    };
    struct compareByDate
    {
        bool operator()(const CStudent & s1, const CStudent & s2) const
        {
            return compareStudentsByBirthDate(s1, s2);
        }
    };

    set<CStudent, compareByEnroll> sortedStudentsByEnroll;
    set<CStudent, compareByDate> sortedStudentsByBirthDate;
    int historyStudentsCount = 0;
};

//---------------------------------------------------------------------------
bool CStudyDept::addStudent(const CStudent & x)
{
    auto iterator = sortedStudentsByEnroll.find(x);
    if (iterator != sortedStudentsByEnroll.end())
        return false;

    CStudent newStudent(x, historyStudentsCount);
    sortedStudentsByEnroll.insert(newStudent);
    sortedStudentsByBirthDate.insert(newStudent);
    historyStudentsCount++;
    return true;
}

//---------------------------------------------------------------------------
bool CStudyDept::delStudent(const CStudent & x)
{
    auto position = sortedStudentsByEnroll.find(x);
    if (position == sortedStudentsByEnroll.end())
        return false;

    sortedStudentsByEnroll.erase(x);
    sortedStudentsByBirthDate.erase(x);

    return true;
}

//---------------------------------------------------------------------------
std::list<CStudent> CStudyDept::search(const CFilter & flt, const CSort & sortOpt) const
{
    auto compare = [&sortOpt](const CStudent & lhs, const CStudent & rhs)
    {
        for (auto sortKey : sortOpt.sortKeys)
        {
            strong_ordering compareBirthDate = (lhs.m_BirthDate <=> rhs.m_BirthDate);
            switch (sortKey.first)
            {
                case ESortKey::NAME:
                    if (lhs.m_StringName != rhs.m_StringName)
                        return sortKey.second ? lhs.m_StringName < rhs.m_StringName : lhs.m_StringName >
                                                                                      rhs.m_StringName;
                    break;

                case ESortKey::BIRTH_DATE:
                    if (compareBirthDate == strong_ordering::less)
                        return sortKey.second;
                    if (compareBirthDate == strong_ordering::greater)
                        return !sortKey.second;
                    break;

                case ESortKey::ENROLL_YEAR:
                    if (lhs.m_EnrollYear != rhs.m_EnrollYear)
                        return sortKey.second ? lhs.m_EnrollYear < rhs.m_EnrollYear : lhs.m_EnrollYear >
                                                                                      rhs.m_EnrollYear;
                    break;
            }
        }
        return lhs.m_HistoryId < rhs.m_HistoryId;
    };

    if (!flt.m_Valid)
        return {};

    list<CStudent> sortedStudents;
    CDate maxDate = CDate(INT_MAX, INT_MAX, INT_MAX);
    CDate minDate = CDate(-1, -1, -1);

    if (flt.m_EnrolledAfter > -1 ||
        flt.m_EnrolledBefore < INT_MAX)
    {
        auto shiftEnroll1 = sortedStudentsByEnroll.lower_bound(CStudent(flt.m_EnrolledAfter + 1));
        auto shiftEnroll2 = sortedStudentsByEnroll.lower_bound(CStudent(flt.m_EnrolledBefore));

        if (shiftEnroll1 == sortedStudentsByEnroll.end() ||
            shiftEnroll2 == sortedStudentsByEnroll.begin())
            return {};

        for (auto i = shiftEnroll1; i != shiftEnroll2; i++)
            if (flt.birthMatch(i->m_BirthDate) && flt.nameMatch(i->m_Name))
                sortedStudents.push_back(*i);

        sortedStudents.sort(compare);
    }
    else if ((flt.m_BornAfter >= minDate) ||
            (flt.m_BornBefore <= maxDate))
    {
        auto shiftDate1 = sortedStudentsByBirthDate.upper_bound(CStudent(flt.m_BornAfter));
        auto shiftDate2 = sortedStudentsByBirthDate.lower_bound(CStudent(flt.m_BornBefore));

        if (shiftDate1 == sortedStudentsByEnroll.end() ||
            shiftDate2 == sortedStudentsByEnroll.begin())
            return {};

        for (auto i = shiftDate1; i != shiftDate2; i++)
            if (flt.nameMatch(i->m_Name) && flt.birthMatch(i->m_BirthDate))
                sortedStudents.push_back(*i);

        sortedStudents.sort(compare);
    }
    else
    {
        for (const auto & student : sortedStudentsByEnroll)
            if (flt.nameMatch(student.m_Name))
                sortedStudents.push_back(student);

        sortedStudents.sort(compare);
    }

    return sortedStudents;
}

//---------------------------------------------------------------------------
std::set<std::string> CStudyDept::suggest(const string & name) const
{
    set<string> names;
    set<string> currentName;
    set<string> subName;

    nameToSet(subName, name);

    for (auto & student : sortedStudentsByEnroll)
    {
        nameToSet(currentName, student.m_StringName);
        if (includes(currentName.begin(), currentName.end(), subName.begin(), subName.end()))
            names.insert(student.m_StringName);
        currentName.clear();
    }

    return names;
}


//---------------------------------------------------------------------------
bool CStudyDept::compareStudentsByBirthDate(const CStudent & s1, const CStudent & s2)
{
    if (s1.m_BirthDate != s2.m_BirthDate)
        return s1.m_BirthDate < s2.m_BirthDate;
    else if (s1.m_Name != s2.m_Name)
        return s1.m_Name < s2.m_Name;
    else if (s1.m_StringName != s2.m_StringName)
        return s1.m_StringName < s2.m_StringName;
    else if (s1.m_EnrollYear != s2.m_EnrollYear)
        return s1.m_EnrollYear < s2.m_EnrollYear;
    else
        return false;
}

//---------------------------------------------------------------------------
bool CStudyDept::compareStudentsByEnroll(const CStudent & s1, const CStudent & s2)
{
    if (s1.m_EnrollYear != s2.m_EnrollYear)
        return s1.m_EnrollYear < s2.m_EnrollYear;
    else if (s1.m_Name != s2.m_Name)
        return s1.m_Name < s2.m_Name;
    else if (s1.m_StringName != s2.m_StringName)
        return s1.m_StringName < s2.m_StringName;
    else if (s1.m_BirthDate != s2.m_BirthDate)
        return s1.m_BirthDate < s2.m_BirthDate;
    else
        return false;
}

#ifndef __PROGTEST__

int main(void)
{
    CStudyDept x0;
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1980, 4, 11), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1980, 4, 11), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1980, 4, 11), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1980, 4, 11), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1997, 6, 17), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1997, 6, 17), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1980, 4, 11), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1980, 4, 11), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1980, 4, 11), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1980, 4, 11), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1997, 6, 17), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1997, 6, 17), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1997, 6, 17), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1997, 6, 17), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1997, 6, 17), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1997, 6, 17), 2016)));
    assert (x0.addStudent(CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014)));
    assert (x0.addStudent(CStudent("John Taylor", CDate(1981, 6, 30), 2012)));
    assert (x0.addStudent(CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)));
    assert (x0.addStudent(CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1982, 7, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 8, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 17), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2012)));
    assert (x0.addStudent(CStudent("Bond James", CDate(1981, 7, 16), 2013)));
    assert (x0.search(CFilter(), CSort()) ==
            (std::list<CStudent>{CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                                 CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                                 CStudent("Peter Taylor", CDate(1982, 2, 23), 2011),
                                 CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                                 CStudent("James Bond", CDate(1981, 7, 16), 2013),
                                 CStudent("James Bond", CDate(1982, 7, 16), 2013),
                                 CStudent("James Bond", CDate(1981, 8, 16), 2013),
                                 CStudent("James Bond", CDate(1981, 7, 17), 2013),
                                 CStudent("James Bond", CDate(1981, 7, 16), 2012),
                                 CStudent("Bond James", CDate(1981, 7, 16), 2013)}));
    assert (x0.search(CFilter(), CSort().addKey(ESortKey::NAME, true)) ==
            (std::list<CStudent>{CStudent("Bond James", CDate(1981, 7, 16), 2013),
                                 CStudent("James Bond", CDate(1981, 7, 16), 2013),
                                 CStudent("James Bond", CDate(1982, 7, 16), 2013),
                                 CStudent("James Bond", CDate(1981, 8, 16), 2013),
                                 CStudent("James Bond", CDate(1981, 7, 17), 2013),
                                 CStudent("James Bond", CDate(1981, 7, 16), 2012),
                                 CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                                 CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                                 CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                                 CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)}));
    assert (x0.search(CFilter(), CSort().addKey(ESortKey::NAME, false)) ==
            (std::list<CStudent>{CStudent("Peter Taylor", CDate(1982, 2, 23), 2011),
                                 CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                                 CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                                 CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                                 CStudent("James Bond", CDate(1981, 7, 16), 2013),
                                 CStudent("James Bond", CDate(1982, 7, 16), 2013),
                                 CStudent("James Bond", CDate(1981, 8, 16), 2013),
                                 CStudent("James Bond", CDate(1981, 7, 17), 2013),
                                 CStudent("James Bond", CDate(1981, 7, 16), 2012),
                                 CStudent("Bond James", CDate(1981, 7, 16), 2013)}));
    assert (x0.search(CFilter(),
                      CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
                              ESortKey::NAME, true)) ==
            (std::list<CStudent>{CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                                 CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                                 CStudent("James Bond", CDate(1982, 7, 16), 2013),
                                 CStudent("James Bond", CDate(1981, 8, 16), 2013),
                                 CStudent("James Bond", CDate(1981, 7, 17), 2013),
                                 CStudent("Bond James", CDate(1981, 7, 16), 2013),
                                 CStudent("James Bond", CDate(1981, 7, 16), 2013),
                                 CStudent("James Bond", CDate(1981, 7, 16), 2012),
                                 CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                                 CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)}));
    assert (x0.search(CFilter().name("james bond"),
                      CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
                              ESortKey::NAME, true)) ==
            (std::list<CStudent>{CStudent("James Bond", CDate(1982, 7, 16), 2013),
                                 CStudent("James Bond", CDate(1981, 8, 16), 2013),
                                 CStudent("James Bond", CDate(1981, 7, 17), 2013),
                                 CStudent("Bond James", CDate(1981, 7, 16), 2013),
                                 CStudent("James Bond", CDate(1981, 7, 16), 2013),
                                 CStudent("James Bond", CDate(1981, 7, 16), 2012)}));
    assert (x0.search(CFilter().bornAfter(CDate(1980, 4, 11)).bornBefore(CDate(1983, 7, 13)).name("John Taylor").name(
            "james BOND"), CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
            ESortKey::NAME, true)) == (std::list<CStudent>{CStudent("James Bond", CDate(1982, 7, 16), 2013),
                                                           CStudent("James Bond", CDate(1981, 8, 16), 2013),
                                                           CStudent("James Bond", CDate(1981, 7, 17), 2013),
                                                           CStudent("Bond James", CDate(1981, 7, 16), 2013),
                                                           CStudent("James Bond", CDate(1981, 7, 16), 2013),
                                                           CStudent("James Bond", CDate(1981, 7, 16), 2012),
                                                           CStudent("John Taylor", CDate(1981, 6, 30), 2012)}));
    assert (x0.search(CFilter().name("james"), CSort().addKey(ESortKey::NAME, true)) == (std::list<CStudent>{}));
    assert (x0.suggest("peter") == (std::set<std::string>{"John Peter Taylor", "Peter John Taylor", "Peter Taylor"}));
    assert (x0.suggest("bond") == (std::set<std::string>{"Bond James", "James Bond"}));
    assert (x0.suggest("peter joHn") == (std::set<std::string>{"John Peter Taylor", "Peter John Taylor"}));
    assert (x0.suggest("peter joHn bond") == (std::set<std::string>{}));
    assert (x0.suggest("pete") == (std::set<std::string>{}));
    assert (x0.suggest("peter joHn PETER") == (std::set<std::string>{"John Peter Taylor", "Peter John Taylor"}));
    assert (!x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.delStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.search(CFilter().bornAfter(CDate(1980, 4, 11)).bornBefore(CDate(1983, 7, 13)).name("John Taylor").name(
            "james BOND"), CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
            ESortKey::NAME, true)) == (std::list<CStudent>{CStudent("James Bond", CDate(1982, 7, 16), 2013),
                                                           CStudent("James Bond", CDate(1981, 8, 16), 2013),
                                                           CStudent("James Bond", CDate(1981, 7, 17), 2013),
                                                           CStudent("Bond James", CDate(1981, 7, 16), 2013),
                                                           CStudent("James Bond", CDate(1981, 7, 16), 2012),
                                                           CStudent("John Taylor", CDate(1981, 6, 30), 2012)}));
    assert (!x0.delStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
