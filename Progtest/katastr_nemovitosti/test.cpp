#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
#endif /* __PROGTEST__ */

using namespace std;

struct Land
{
  Land(string cityName, string addr, string reg, unsigned int id) : region(reg), id(id), city(cityName), address(addr), owner(""){};
  Land(string owner) : region(""), id(0), city(""), address(""), owner(owner){};

  string region;
  unsigned int id;
  string city;
  string address;
  string owner;
};

class CIterator
{
public:
  CIterator(const vector<Land> &land)
  {
    toIterate = land;
    it = toIterate.begin();
  };

  bool atEnd() const { return it == toIterate.end(); };
  void next() { it += 1; };
  std::string city() const { return it->city; };
  std::string addr() const { return it->address; };
  std::string region() const { return it->region; };
  unsigned id() const { return it->id; };
  std::string owner() const { return it->owner; };

private:
  vector<Land> toIterate;
  vector<Land>::iterator it;
};

class CLandRegister
{
public:
  bool add(const std::string &city,
           const std::string &addr,
           const std::string &region,
           unsigned int id)
  {
    Land newLand = Land(city, addr, region, id);

    if (find_if(sortedByRegion.begin(), sortedByRegion.end(), [newLand](const Land &land)
                { return CLandRegister::findLand(land, newLand); }) != sortedByRegion.end())
      return false;

    auto it = lower_bound(sortedByRegion.begin(), sortedByRegion.end(), newLand, &CLandRegister::compareByRegion);
    sortedByRegion.insert(it, newLand);

    it = lower_bound(sortedByCity.begin(), sortedByCity.end(), newLand, &CLandRegister::compareByCity);
    sortedByCity.insert(it, newLand);

    sortedByOwner.insert(sortedByOwner.begin(), newLand);
    return true;
  };

  bool del(const std::string &city,
           const std::string &addr)
  {
    Land findLand = Land(city, addr, "", 0);
    auto it = find_if(sortedByCity.begin(), sortedByCity.end(), [findLand](const Land &land)
                      { return CLandRegister::findLand(land, findLand); });

    if (it == sortedByCity.end())
      return false;

    sortedByCity.erase(it);

    it = find_if(sortedByRegion.begin(), sortedByRegion.end(), [findLand](const Land &land)
                 { return CLandRegister::findLand(land, findLand); });
    sortedByRegion.erase(it);

    it = find_if(sortedByOwner.begin(), sortedByOwner.end(), [findLand](const Land &land)
                 { return CLandRegister::findLand(land, findLand); });
    sortedByOwner.erase(it);

    return true;
  };

  bool del(const std::string &region,
           unsigned int id)
  {
    Land findLand = Land("", "", region, id);
    auto it = find_if(sortedByRegion.begin(), sortedByRegion.end(), [findLand](const Land &land)
                      { return CLandRegister::findLand(land, findLand); });

    if (it == sortedByRegion.end())
      return false;

    sortedByRegion.erase(it);

    it = find_if(sortedByCity.begin(), sortedByCity.end(), [findLand](const Land &land)
                 { return CLandRegister::findLand(land, findLand); });
    sortedByCity.erase(it);

    it = find_if(sortedByOwner.begin(), sortedByOwner.end(), [findLand](const Land &land)
                 { return CLandRegister::findLand(land, findLand); });
    sortedByOwner.erase(it);

    return true;
  };

  bool getOwner(const std::string &city,
                const std::string &addr,
                std::string &owner) const
  {
    Land findLand = Land(city, addr, "", 0);
    auto it = find_if(sortedByCity.begin(), sortedByCity.end(), [findLand](const Land &land)
                      { return CLandRegister::findLand(land, findLand); });

    if (it == sortedByCity.end())
      return false;

    owner = it->owner;
    return true;
  };

  bool getOwner(const std::string &region,
                unsigned int id,
                std::string &owner) const
  {
    Land findLand = Land("", "", region, id);
    auto it = find_if(sortedByRegion.begin(), sortedByRegion.end(), [findLand](const Land &land)
                      { return CLandRegister::findLand(land, findLand); });

    if (it == sortedByRegion.end())
      return false;

    owner = it->owner;
    return true;
  };

  bool newOwner(const std::string &city,
                const std::string &addr,
                const std::string &owner)
  {
    Land findLand = Land(city, addr, "", 0);
    auto it = find_if(sortedByCity.begin(), sortedByCity.end(), [findLand](const Land &land)
                      { return CLandRegister::findLand(land, findLand); });

    if (it == sortedByCity.end() || it->owner == owner)
      return false;

    Land purchasedLand = *it;
    it->owner = owner;

    it = find_if(sortedByRegion.begin(), sortedByRegion.end(), [purchasedLand](const Land &land)
                 { return CLandRegister::findLand(land, purchasedLand); });
    it->owner = owner;

    it = find_if(sortedByOwner.begin(), sortedByOwner.end(), [purchasedLand](const Land &land)
                 { return CLandRegister::findLand(land, purchasedLand); });
    sortedByOwner.erase(it);

    purchasedLand.owner = owner;

    it = lower_bound(sortedByOwner.begin(), sortedByOwner.end(), purchasedLand, &CLandRegister::compareByOwner);
    sortedByOwner.insert(it, purchasedLand);

    return true;
  };

  bool newOwner(const std::string &region,
                unsigned int id,
                const std::string &owner)
  {
    Land findLand = Land("", "", region, id);
    auto it = find_if(sortedByRegion.begin(), sortedByRegion.end(), [findLand](const Land &land)
                      { return CLandRegister::findLand(land, findLand); });

    if (it == sortedByRegion.end() || it->owner == owner)
      return false;

    Land purchasedLand = *it;
    it->owner = owner;

    it = find_if(sortedByCity.begin(), sortedByCity.end(), [purchasedLand](const Land &land)
                 { return CLandRegister::findLand(land, purchasedLand); });
    it->owner = owner;

    it = find_if(sortedByOwner.begin(), sortedByOwner.end(), [purchasedLand](const Land &land)
                 { return CLandRegister::findLand(land, purchasedLand); });
    sortedByOwner.erase(it);

    purchasedLand.owner = owner;

    it = lower_bound(sortedByOwner.begin(), sortedByOwner.end(), purchasedLand, &CLandRegister::compareByOwner);
    sortedByOwner.insert(it, purchasedLand);

    return true;
  };

  size_t count(const std::string &owner) const
  {
    Land land(owner);
    auto it1 = lower_bound(sortedByOwner.begin(), sortedByOwner.end(), land, &CLandRegister::compareByOwner);
    auto it2 = upper_bound(sortedByOwner.begin(), sortedByOwner.end(), land, &CLandRegister::compareByOwner);
    size_t count = distance(it1, it2);
    return count;
  };

  CIterator listByAddr() const { return CIterator(sortedByCity); };

  CIterator listByOwner(const std::string &owner) const
  {
    Land land(owner);
    auto it1 = lower_bound(sortedByOwner.begin(), sortedByOwner.end(), land, &CLandRegister::compareByOwner);
    auto it2 = upper_bound(sortedByOwner.begin(), sortedByOwner.end(), land, &CLandRegister::compareByOwner);
    vector<Land> ownerLand(it1, it2);
    reverse(ownerLand.begin(), ownerLand.end());
    return CIterator(ownerLand);
  };

private:
  static string toLow(const string &s)
  {
    string low = s;
    for (char &c : low)
      c = tolower(c);
    return low;
  };

  static bool compareByRegion(const Land &first, const Land &second)
  {
    if (first.region != second.region)
      return first.region < second.region;
    return first.id < second.id;
  };

  static bool compareByCity(const Land &first, const Land &second)
  {
    if (first.city != second.city)
      return first.city < second.city;
    return first.address < second.address;
  };

  static bool compareByOwner(const Land &first, const Land &second) { return toLow(first.owner) < toLow(second.owner); }

  static bool findLand(const Land &land, const Land &targetLand)
  {
    return (land.region == targetLand.region && land.id == targetLand.id) || (land.city == targetLand.city && land.address == targetLand.address);
  };

  vector<Land> sortedByCity;
  vector<Land> sortedByRegion;
  vector<Land> sortedByOwner;
};
#ifndef __PROGTEST__
static void test0()
{
  CLandRegister x;
  std::string owner;

  assert(x.add("Prague", "Thakurova", "Dejvice", 12345));
  assert(x.add("Prague", "Evropska", "Vokovice", 12345));
  assert(x.add("Prague", "Technicka", "Dejvice", 9873));
  assert(x.add("Plzen", "Evropska", "Plzen mesto", 78901));
  assert(x.add("Liberec", "Evropska", "Librec", 4552));
  CIterator i0 = x.listByAddr();
  assert(!i0.atEnd() && i0.city() == "Liberec" && i0.addr() == "Evropska" && i0.region() == "Librec" && i0.id() == 4552 && i0.owner() == "");
  i0.next();
  assert(!i0.atEnd() && i0.city() == "Plzen" && i0.addr() == "Evropska" && i0.region() == "Plzen mesto" && i0.id() == 78901 && i0.owner() == "");
  i0.next();
  assert(!i0.atEnd() && i0.city() == "Prague" && i0.addr() == "Evropska" && i0.region() == "Vokovice" && i0.id() == 12345 && i0.owner() == "");
  i0.next();
  assert(!i0.atEnd() && i0.city() == "Prague" && i0.addr() == "Technicka" && i0.region() == "Dejvice" && i0.id() == 9873 && i0.owner() == "");
  i0.next();
  assert(!i0.atEnd() && i0.city() == "Prague" && i0.addr() == "Thakurova" && i0.region() == "Dejvice" && i0.id() == 12345 && i0.owner() == "");
  i0.next();
  assert(i0.atEnd());

  assert(x.count("") == 5);
  CIterator i1 = x.listByOwner("");
  assert(!i1.atEnd() && i1.city() == "Prague" && i1.addr() == "Thakurova" && i1.region() == "Dejvice" && i1.id() == 12345 && i1.owner() == "");
  i1.next();
  assert(!i1.atEnd() && i1.city() == "Prague" && i1.addr() == "Evropska" && i1.region() == "Vokovice" && i1.id() == 12345 && i1.owner() == "");
  i1.next();
  assert(!i1.atEnd() && i1.city() == "Prague" && i1.addr() == "Technicka" && i1.region() == "Dejvice" && i1.id() == 9873 && i1.owner() == "");
  i1.next();
  assert(!i1.atEnd() && i1.city() == "Plzen" && i1.addr() == "Evropska" && i1.region() == "Plzen mesto" && i1.id() == 78901 && i1.owner() == "");
  i1.next();
  assert(!i1.atEnd() && i1.city() == "Liberec" && i1.addr() == "Evropska" && i1.region() == "Librec" && i1.id() == 4552 && i1.owner() == "");
  i1.next();
  assert(i1.atEnd());

  assert(x.count("CVUT") == 0);
  CIterator i2 = x.listByOwner("CVUT");
  assert(i2.atEnd());

  assert(x.newOwner("Prague", "Thakurova", "CVUT"));
  assert(x.newOwner("Dejvice", 9873, "CVUT"));
  assert(x.newOwner("Plzen", "Evropska", "Anton Hrabis"));
  assert(x.newOwner("Librec", 4552, "Cvut"));
  assert(x.getOwner("Prague", "Thakurova", owner) && owner == "CVUT");
  assert(x.getOwner("Dejvice", 12345, owner) && owner == "CVUT");
  assert(x.getOwner("Prague", "Evropska", owner) && owner == "");
  assert(x.getOwner("Vokovice", 12345, owner) && owner == "");
  assert(x.getOwner("Prague", "Technicka", owner) && owner == "CVUT");
  assert(x.getOwner("Dejvice", 9873, owner) && owner == "CVUT");
  assert(x.getOwner("Plzen", "Evropska", owner) && owner == "Anton Hrabis");
  assert(x.getOwner("Plzen mesto", 78901, owner) && owner == "Anton Hrabis");
  assert(x.getOwner("Liberec", "Evropska", owner) && owner == "Cvut");
  assert(x.getOwner("Librec", 4552, owner) && owner == "Cvut");
  CIterator i3 = x.listByAddr();
  assert(!i3.atEnd() && i3.city() == "Liberec" && i3.addr() == "Evropska" && i3.region() == "Librec" && i3.id() == 4552 && i3.owner() == "Cvut");
  i3.next();
  assert(!i3.atEnd() && i3.city() == "Plzen" && i3.addr() == "Evropska" && i3.region() == "Plzen mesto" && i3.id() == 78901 && i3.owner() == "Anton Hrabis");
  i3.next();
  assert(!i3.atEnd() && i3.city() == "Prague" && i3.addr() == "Evropska" && i3.region() == "Vokovice" && i3.id() == 12345 && i3.owner() == "");
  i3.next();
  assert(!i3.atEnd() && i3.city() == "Prague" && i3.addr() == "Technicka" && i3.region() == "Dejvice" && i3.id() == 9873 && i3.owner() == "CVUT");
  i3.next();
  assert(!i3.atEnd() && i3.city() == "Prague" && i3.addr() == "Thakurova" && i3.region() == "Dejvice" && i3.id() == 12345 && i3.owner() == "CVUT");
  i3.next();
  assert(i3.atEnd());

  assert(x.count("cvut") == 3);
  CIterator i4 = x.listByOwner("cVuT");
  assert(!i4.atEnd() && i4.city() == "Prague" && i4.addr() == "Thakurova" && i4.region() == "Dejvice" && i4.id() == 12345 && i4.owner() == "CVUT");
  i4.next();
  assert(!i4.atEnd() && i4.city() == "Prague" && i4.addr() == "Technicka" && i4.region() == "Dejvice" && i4.id() == 9873 && i4.owner() == "CVUT");
  i4.next();
  assert(!i4.atEnd() && i4.city() == "Liberec" && i4.addr() == "Evropska" && i4.region() == "Librec" && i4.id() == 4552 && i4.owner() == "Cvut");
  i4.next();
  assert(i4.atEnd());

  assert(x.newOwner("Plzen mesto", 78901, "CVut"));
  assert(x.count("CVUT") == 4);
  CIterator i5 = x.listByOwner("CVUT");
  assert(!i5.atEnd() && i5.city() == "Prague" && i5.addr() == "Thakurova" && i5.region() == "Dejvice" && i5.id() == 12345 && i5.owner() == "CVUT");
  i5.next();
  assert(!i5.atEnd() && i5.city() == "Prague" && i5.addr() == "Technicka" && i5.region() == "Dejvice" && i5.id() == 9873 && i5.owner() == "CVUT");
  i5.next();
  assert(!i5.atEnd() && i5.city() == "Liberec" && i5.addr() == "Evropska" && i5.region() == "Librec" && i5.id() == 4552 && i5.owner() == "Cvut");
  i5.next();
  assert(!i5.atEnd() && i5.city() == "Plzen" && i5.addr() == "Evropska" && i5.region() == "Plzen mesto" && i5.id() == 78901 && i5.owner() == "CVut");
  i5.next();
  assert(i5.atEnd());

  assert(x.del("Liberec", "Evropska"));
  assert(x.del("Plzen mesto", 78901));
  assert(x.count("cvut") == 2);
  CIterator i6 = x.listByOwner("cVuT");
  assert(!i6.atEnd() && i6.city() == "Prague" && i6.addr() == "Thakurova" && i6.region() == "Dejvice" && i6.id() == 12345 && i6.owner() == "CVUT");
  i6.next();
  assert(!i6.atEnd() && i6.city() == "Prague" && i6.addr() == "Technicka" && i6.region() == "Dejvice" && i6.id() == 9873 && i6.owner() == "CVUT");
  i6.next();
  assert(i6.atEnd());

  assert(x.add("Liberec", "Evropska", "Librec", 4552));
}

static void test1()
{
  CLandRegister x;
  std::string owner;

  assert(x.add("Prague", "Thakurova", "Dejvice", 12345));
  assert(x.add("Prague", "Evropska", "Vokovice", 12345));
  assert(x.add("Prague", "Technicka", "Dejvice", 9873));
  assert(!x.add("Prague", "Technicka", "Hradcany", 7344));
  assert(!x.add("Brno", "Bozetechova", "Dejvice", 9873));
  assert(!x.getOwner("Prague", "THAKUROVA", owner));
  assert(!x.getOwner("Hradcany", 7343, owner));
  CIterator i0 = x.listByAddr();
  assert(!i0.atEnd() && i0.city() == "Prague" && i0.addr() == "Evropska" && i0.region() == "Vokovice" && i0.id() == 12345 && i0.owner() == "");
  i0.next();
  assert(!i0.atEnd() && i0.city() == "Prague" && i0.addr() == "Technicka" && i0.region() == "Dejvice" && i0.id() == 9873 && i0.owner() == "");
  i0.next();
  assert(!i0.atEnd() && i0.city() == "Prague" && i0.addr() == "Thakurova" && i0.region() == "Dejvice" && i0.id() == 12345 && i0.owner() == "");
  i0.next();
  assert(i0.atEnd());

  assert(x.newOwner("Prague", "Thakurova", "CVUT"));
  assert(!x.newOwner("Prague", "technicka", "CVUT"));
  assert(!x.newOwner("prague", "Technicka", "CVUT"));
  assert(!x.newOwner("dejvice", 9873, "CVUT"));
  assert(!x.newOwner("Dejvice", 9973, "CVUT"));
  assert(!x.newOwner("Dejvice", 12345, "CVUT"));
  assert(x.count("CVUT") == 1);
  CIterator i1 = x.listByOwner("CVUT");
  assert(!i1.atEnd() && i1.city() == "Prague" && i1.addr() == "Thakurova" && i1.region() == "Dejvice" && i1.id() == 12345 && i1.owner() == "CVUT");
  i1.next();
  assert(i1.atEnd());

  assert(!x.del("Brno", "Technicka"));
  assert(!x.del("Karlin", 9873));
  assert(x.del("Prague", "Technicka"));
  assert(!x.del("Prague", "Technicka"));
  assert(!x.del("Dejvice", 9873));
}

int main(void)
{
  test0();
  test1();
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
