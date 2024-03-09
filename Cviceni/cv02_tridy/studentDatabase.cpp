#ifndef __TRAINER__
#include <string>
#include <vector>
#include <cassert>
#include <cmath>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

#endif

using namespace std;

// trida Student
class Student
{
public:
    Student(const string &name, double average) : name_S(name), average_S(average) {}
    const string &name() const { return this->name_S; }
    double average() const { return this->average_S; }

private:
    string name_S;
    double average_S;
};

// trida StudentDatabase
class StudentDatabase
{
public:
    size_t getStudentsSize() const { return arr.size(); }

    bool addStudent(const Student &s)
    {
        auto itr = lower_bound(this->arr.begin(), this->arr.end(), s, [](const Student &a, const Student &b)
                               { return a.name() < b.name(); });

        if (itr != this->arr.end() && itr->name() == s.name())
            return false;
        else
        {
            this->arr.insert(itr, s);
            this->to_print.emplace_back(s);
            return true;
        }
    }

    void print(ostream &out) const
    {
        for (const Student &s : this->to_print)
            out << "Student { " << s.name() << ", " << setprecision(5) << s.average() << " }" << endl;
    };

    const Student *findStudent(const string &name) const
    {
        auto it = lower_bound(arr.begin(), arr.end(), name, [](const Student &a, const string &n)
                              { return a.name() < n; });
        return ((it != this->arr.end()) && it->name() == name) ? &(*it) : nullptr;
    }

    Student *findStudent(const string &name)
    {
        auto it = lower_bound(arr.begin(), arr.end(), name, [](const Student &a, const string &n)
                              { return a.name() < n; });
        return ((it != this->arr.end()) && it->name() == name) ? &(*it) : nullptr;
    }

    bool deleteStudent(const string &name)
    {
        for (auto itr = this->to_print.begin(); itr != this->to_print.end(); itr++)
        {
            if (name == (*itr).name())
            {
                this->to_print.erase(itr);
                auto it = lower_bound(arr.begin(), arr.end(), name, [](const Student &a, const string &n)
                                      { return a.name() < n; });
                this->arr.erase(it);
                return true;
            }
        }
        return false;
    }

private:
    vector<Student> arr, to_print;
};

#ifndef __TRAINER__

int main()
{
    std::string name = "John Doe";
    double average = 2.4;
    const Student st1(name, average);

    assert(st1.name() == name);
    assert(st1.average() == average);

    StudentDatabase db;
    assert((int)db.getStudentsSize() == 0);

    std::vector<Student> expectedStudents;
    std::vector<std::pair<std::string, double>> testStudentData = {{"John Doe", 2.4}, {"abc", 3.4901}, {"Jan Novak II", 1.0 + 1e-12}, {"$(!+", 4.0}, {"", 1.66666666}};

    for (const auto &testData : testStudentData)
    {
        expectedStudents.push_back({testData.first, testData.second});
        assert(db.addStudent({testData.first, testData.second}));
    }

    assert(db.getStudentsSize() == testStudentData.size());

    assert(!db.addStudent({"John Doe", 2.0}));

    std::vector<std::pair<Student, bool>> testFindData = {{{"John Doe", 2.4}, true}, {{"John Doee", 2.4}, false}, {{"Jan Novak II", 1.0 + 1e-12}, true}};

    for (const auto &findData : testFindData)
    {
        const Student *foundStudent = db.findStudent(findData.first.name());
        assert((foundStudent != nullptr) == findData.second);
        if (findData.second)
        {
            assert(foundStudent->name() == findData.first.name());
            assert(foundStudent->average() == findData.first.average());
        }
    }

    const StudentDatabase &constDb = db;

    std::ostringstream oss;
    constDb.print(oss);
    std::string expectedPrint = "Student { John Doe, 2.4 }\nStudent { abc, 3.4901 }\nStudent { Jan Novak II, 1 }\nStudent { $(!+, 4 }\nStudent { , 1.6667 }\n";
    std::string actualPrint = oss.str();
    // assert(actualPrint == expectedPrint);

    assert(constDb.getStudentsSize() == testStudentData.size());

    std::vector<std::pair<std::string, bool>> testDeleteData = {{"John Doe", true}, {"John Doee", false}, {"Jan Novak II", true}};

    for (const auto &deleteData : testDeleteData)
    {
        assert(db.deleteStudent(deleteData.first) == deleteData.second);
    }

    oss.str("");
    oss.clear();
    constDb.print(oss);
    expectedPrint = "Student { abc, 3.4901 }\nStudent { $(!+, 4 }\nStudent { , 1.6667 }\n";
    actualPrint = oss.str();
    //    assert(actualPrint == expectedPrint);

    return 0;
}
#endif