#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

using namespace std;

class CTimeStamp
{
public:
  CTimeStamp(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second)
      : year(year), month(month), day(day), hour(hour), minute(minute), second(second){};

  bool operator<(CTimeStamp &right)
  {
    if (year != right.year)
      return year < right.year;
    if (month != right.month)
      return month < right.month;
    if (day != right.day)
      return day < right.day;
    if (hour != right.hour)
      return hour < right.hour;
    if (minute != right.minute)
      return minute < right.minute;
    if (second != right.second)
      return second < right.second;
    return true;
  };

  unsigned int year, month, day, hour, minute, second;
};

class CContact
{
public:
  CContact(CTimeStamp r, int p1, int p2)
      : person1(p1), person2(p2), razitko(r){};

  int person1, person2;
  CTimeStamp razitko;
};

class CEFaceMask
{
public:
  CEFaceMask &addContact(CContact contact)
  {
    contacts.emplace_back(contact);
    return *this;
  };

  vector<int> listContacts(int person) const
  {
    vector<int> to_return;
    for (auto contact : contacts)
    {
      if (contact.person1 == person)
      {
        if (contact.person2 != person && find_if(to_return.begin(), to_return.end(), [contact](int left)
                                                 { return left == contact.person2; }) == to_return.end())
          to_return.emplace_back(contact.person2);
      }
      if (contact.person2 == person)
      {
        if (contact.person1 != person && find_if(to_return.begin(), to_return.end(), [contact](int left)
                                                 { return left == contact.person1; }) == to_return.end())
          to_return.emplace_back(contact.person1);
      }
    }
    return vector<int>(to_return.begin(), to_return.end());
  };

  vector<int> listContacts(int person, CTimeStamp from, CTimeStamp to) const
  {
    vector<int> to_return;
    for (auto contact : contacts)
    {
      if (contact.person1 == person)
      {
        if (contact.person2 != person && from < contact.razitko && contact.razitko < to && find_if(to_return.begin(), to_return.end(), [contact](int left)
                                                                                                   { return left == contact.person2; }) == to_return.end())
          to_return.emplace_back(contact.person2);
      }
      if (contact.person2 == person)
      {
        if (contact.person1 != person && from < contact.razitko && contact.razitko < to && find_if(to_return.begin(), to_return.end(), [contact](int left)
                                                                                                   { return left == contact.person1; }) == to_return.end())
          to_return.emplace_back(contact.person1);
      }
    }
    return vector<int>(to_return.begin(), to_return.end());
  };

private:
  vector<CContact> contacts;
};

#ifndef __PROGTEST__
int main()
{
  CEFaceMask test;

  test.addContact(CContact(CTimeStamp(2021, 1, 10, 12, 40, 10), 123456789, 999888777));
  test.addContact(CContact(CTimeStamp(2021, 1, 12, 12, 40, 10), 123456789, 111222333))
      .addContact(CContact(CTimeStamp(2021, 2, 5, 15, 30, 28), 999888777, 555000222));
  test.addContact(CContact(CTimeStamp(2021, 2, 21, 18, 0, 0), 123456789, 999888777));
  test.addContact(CContact(CTimeStamp(2021, 1, 5, 18, 0, 0), 123456789, 456456456));
  test.addContact(CContact(CTimeStamp(2021, 2, 1, 0, 0, 0), 123456789, 123456789));
  assert(test.listContacts(123456789) == (vector<int>{999888777, 111222333, 456456456}));
  assert(test.listContacts(999888777) == (vector<int>{123456789, 555000222}));
  assert(test.listContacts(191919191) == (vector<int>{}));
  assert(test.listContacts(123456789, CTimeStamp(2021, 1, 5, 18, 0, 0), CTimeStamp(2021, 2, 21, 18, 0, 0)) == (vector<int>{999888777, 111222333, 456456456}));
  assert(test.listContacts(123456789, CTimeStamp(2021, 1, 5, 18, 0, 1), CTimeStamp(2021, 2, 21, 17, 59, 59)) == (vector<int>{999888777, 111222333}));
  assert(test.listContacts(123456789, CTimeStamp(2021, 1, 10, 12, 41, 9), CTimeStamp(2021, 2, 21, 17, 59, 59)) == (vector<int>{111222333}));
  return 0;
}
#endif /* __PROGTEST__ */
