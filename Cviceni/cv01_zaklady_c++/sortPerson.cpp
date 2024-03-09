#ifndef __TRAINER__
#include <algorithm>
#include <cassert>
#include <string>

struct Person
{
    std::string name;           /* Jméno člověka */
    unsigned long long dollars; /* Koruny */
    unsigned pennies;           /* Haléře */
    unsigned cats;              /* Počet koček */
    unsigned age;               /* Věk */
};

#endif

bool cmp(const Person &lhs, const Person &rhs)
{
    if (lhs.dollars != rhs.dollars)
        return lhs.dollars > rhs.dollars;
    else if (lhs.dollars == rhs.dollars && lhs.pennies != rhs.pennies)
        return lhs.pennies > rhs.pennies;
    else if (lhs.dollars == rhs.dollars && lhs.pennies == rhs.pennies && lhs.age != rhs.age)
        return lhs.age < rhs.age;
    else if (lhs.dollars == rhs.dollars && lhs.pennies == rhs.pennies && lhs.age == rhs.age && lhs.cats != rhs.cats)
        return lhs.cats > rhs.cats;
    else
        return lhs.name < rhs.name;
}

#ifndef __TRAINER__

Person *bankSort(Person *people, size_t n)
{
    std::sort(people, people + n, cmp);
    return people;
}

bool personEqual(const Person &lhs, const Person &rhs)
{
    return lhs.name == rhs.name &&
           lhs.dollars == rhs.dollars &&
           lhs.pennies == rhs.pennies &&
           lhs.cats == rhs.cats &&
           lhs.age == rhs.age;
}

bool arrEqual(Person *lhs, size_t n1, Person *rhs, size_t n2)
{
    if (n1 != n2)
        return false;
    for (size_t i = 0; i < n1; i++)
        if (!personEqual(lhs[i], rhs[i]))
            return false;
    return true;
}

int main()
{
    Person a1[] = {{"Honza", 0, 0, 0, 0}, {"Pepa", 1, 0, 0, 0}};
    Person ref1[] = {{"Pepa", 1, 0, 0, 0}, {"Honza", 0, 0, 0, 0}};
    bankSort(a1, sizeof(a1) / sizeof(Person));
    assert(arrEqual(a1, sizeof(a1) / sizeof(Person), ref1, sizeof(ref1) / sizeof(Person)));

    Person a2[] = {{"Borec", 1, 2, 0, 0}, {"VetsiBorec", 1, 3, 0, 0}};
    Person ref2[] = {{"VetsiBorec", 1, 3, 0, 0}, {"Borec", 1, 2, 0, 0}};
    bankSort(a2, sizeof(a2) / sizeof(Person));
    assert(arrEqual(a2, sizeof(a2) / sizeof(Person), ref2, sizeof(ref2) / sizeof(Person)));

    Person a3[] = {{"Ladislav", 1, 3, 0, 50}, {"Jan", 1, 3, 0, 20}};
    Person ref3[] = {{"Jan", 1, 3, 0, 20}, {"Ladislav", 1, 3, 0, 50}};
    bankSort(a3, sizeof(a3) / sizeof(Person));
    assert(arrEqual(a3, sizeof(a3) / sizeof(Person), ref3, sizeof(ref3) / sizeof(Person)));

    Person a4[] = {{"Marcel", 1, 3, 0, 20}, {"Petra", 1, 3, 42, 20}};
    Person ref4[] = {{"Petra", 1, 3, 42, 20}, {"Marcel", 1, 3, 0, 20}};
    bankSort(a4, sizeof(a4) / sizeof(Person));
    assert(arrEqual(a4, sizeof(a4) / sizeof(Person), ref4, sizeof(ref4) / sizeof(Person)));

    Person a5[] = {{"aa", 1, 3, 42, 20}, {"a", 1, 3, 42, 20}};
    Person ref5[] = {{"a", 1, 3, 42, 20}, {"aa", 1, 3, 42, 20}};
    bankSort(a5, sizeof(a5) / sizeof(Person));
    assert(arrEqual(a5, sizeof(a5) / sizeof(Person), ref5, sizeof(ref5) / sizeof(Person)));
}
#endif
