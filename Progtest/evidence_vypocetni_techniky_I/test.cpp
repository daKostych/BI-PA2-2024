#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>
#endif /* __PROGTEST__ */

using namespace std;

class NamedObject;
class Component;

using ANamedObject = unique_ptr<NamedObject>;
using AComponent = unique_ptr<Component>;

//----------------------------------------------------------------------------------------------------------------------
class Component
{
public:
    Component() = default;
    virtual ~Component() = default;

    virtual void printComponent(ostream & os, bool last, bool firstPipe, bool onePC) const = 0;
    virtual Component * cloneObject() const = 0;
};
//----------------------------------------------------------------------------------------------------------------------
class CCPU : public Component
{
public:
    CCPU(unsigned int k, unsigned int f) : Component(),
                                           m_KernelCount(k),
                                           m_Freq(f) {};

    void printComponent(ostream & os, bool last, bool firstPipe, bool onePC) const override;
    Component * cloneObject() const override { return new CCPU(m_KernelCount, m_Freq); };

private:
    unsigned int m_KernelCount;
    unsigned int m_Freq;
};

void CCPU::printComponent(ostream & os, bool last, bool firstPipe, bool onePC) const
{
    if(!onePC)
    {
        os << (firstPipe ? "| "
                         : "  ")
           << (last ? "\\-CPU, "
                    : "+-CPU, ")
           << m_KernelCount << " cores @ " << m_Freq << "MHz\n";
    }
    else
    {
        os << (last ? "\\-CPU, "
                    : "+-CPU, ")
           << m_KernelCount << " cores @ " << m_Freq << "MHz\n";
    }
}
//----------------------------------------------------------------------------------------------------------------------
class CMemory : public Component
{
public:
    CMemory(unsigned int mb) : Component(),
                               m_MiB(mb) {};

    void printComponent(ostream & os, bool last, bool firstPipe, bool onePC) const override;
    Component * cloneObject() const override { return new CMemory(m_MiB); };

private:
    unsigned int m_MiB;
};

void CMemory::printComponent(ostream & os, bool last, bool firstPipe, bool onePC) const
{
    if (!onePC)
    {
        os << (firstPipe ? "| "
                         : "  ")
           << (last ? "\\-Memory, "
                    : "+-Memory, ")
           << m_MiB << " MiB\n";
    }
    else
    {
        os << (last ? "\\-Memory, "
                    : "+-Memory, ")
           << m_MiB << " MiB\n";
    }
}
//----------------------------------------------------------------------------------------------------------------------
class CDisk : public Component
{
public:
    enum DT { SSD, MAGNETIC };
    struct Parcel
    {
        Parcel(string  id, unsigned int cap) : parcelID(std::move(id)),
                                               parcelCapacity(cap) {}
        string parcelID;
        unsigned int parcelCapacity;
    };

    CDisk(DT type, unsigned int gb) : Component(),
                                      m_Type(type),
                                      m_Capacity(gb) {}
    CDisk(const CDisk & disk);
    CDisk & operator=(const CDisk & disk);

    CDisk & addPartition(unsigned int capacity, const string & id);
    void printComponent(ostream & os, bool last, bool firstPipe, bool onePC) const override;
    Component * cloneObject() const override { return new CDisk(*this); };

private:
    DT m_Type;
    unsigned int m_Capacity;
    vector<Parcel> m_Parcels{};
};

CDisk::CDisk(const CDisk & disk) : Component(),
                                   m_Type(disk.m_Type),
                                   m_Capacity(disk.m_Capacity)
{
    for (auto & parcel : disk.m_Parcels)
        m_Parcels.push_back(parcel);
}

CDisk & CDisk::operator=(const CDisk & disk)
{
    if (this == &disk)
        return *this;

    CDisk copyToSwap(disk);
    swap(m_Type, copyToSwap.m_Type);
    swap(m_Capacity, copyToSwap.m_Capacity);
    swap(m_Parcels, copyToSwap.m_Parcels);
    return *this;
}

CDisk & CDisk::addPartition(unsigned int capacity, const string & id)
{
    m_Parcels.emplace_back(id, capacity);
    return *this;
}

void CDisk::printComponent(ostream & os, bool last, bool firstPipe, bool onePC) const
{
    string diskType = m_Type == CDisk::SSD ? "SSD"
                                           : "HDD";
    if(!onePC)
    {
        os << (firstPipe ? "| "
                         : "  ")
           << (last ? "\\-"
                    : "+-")
           << diskType << ", " << m_Capacity << " GiB\n";
        for (size_t i = 0; i < m_Parcels.size(); i++)
            os << (firstPipe ? "| "
                             : "  ")
               << (last ? "  "
                        : "| ")
               << ((i == m_Parcels.size() - 1) ? "\\-["
                                               : "+-[")
               << i << "]: " << m_Parcels[i].parcelCapacity << " GiB, " << m_Parcels[i].parcelID << "\n";
    }
    else
    {
        os << (last ? "\\-"
                    : "+-")
           << diskType << ", " << m_Capacity << " GiB\n";
        for (size_t i = 0; i < m_Parcels.size(); i++)
            os << (last ? "  "
                        : "| ")
               << ((i == m_Parcels.size() - 1) ? "\\-["
                                               : "+-[")
               << i << "]: " << m_Parcels[i].parcelCapacity << " GiB, " << m_Parcels[i].parcelID << "\n";
    }
}
//----------------------------------------------------------------------------------------------------------------------
class NamedObject
{
public:
    NamedObject(string name) : m_Name(std::move(name)) {};
    virtual ~NamedObject() = default;

    virtual void printNamedObject(ostream & os, bool last, bool firstPipe, bool onePC) const = 0;
    virtual NamedObject * cloneObject() const = 0;
    virtual string getName() const { return m_Name; };

    friend ostream & operator<<(ostream & os, const NamedObject & object);

protected:
    string m_Name;
};

ostream & operator<<(ostream & os, const NamedObject & object)
{
    object.printNamedObject(os, false, true, false);
    return os;
}
//----------------------------------------------------------------------------------------------------------------------
class CComputer : public NamedObject
{
public:
    CComputer(string name) : NamedObject(std::move(name)) {};
    CComputer(string name, const CComputer & pc);
    CComputer & operator=(const CComputer & pc);

    CComputer & addAddress(const string & addr);
    CComputer & addComponent(const Component & component);
    void printNamedObject(ostream & os, bool last, bool firstPipe, bool onePC) const override;
    NamedObject * cloneObject() const override { return new CComputer(m_Name, *this); };

    friend ostream & operator<<(ostream & os, const CComputer & computer);

private:
    vector<string> m_Addresses;
    vector<AComponent> m_Components;
};

CComputer::CComputer(string name, const CComputer & pc) : NamedObject(std::move(name))
{
    for (const auto & addr : pc.m_Addresses)
        m_Addresses.push_back(addr);
    for (auto & component : pc.m_Components)
        addComponent(*component);
}

CComputer & CComputer::operator=(const CComputer & pc)
{
    if (this == &pc)
        return *this;

    CComputer copyToSwap(pc.m_Name, pc);
    swap(m_Addresses, copyToSwap.m_Addresses);
    swap(m_Components, copyToSwap.m_Components);
    return *this;
}

CComputer & CComputer::addAddress(const string & addr)
{
    m_Addresses.push_back(addr);
    return *this;
}

CComputer & CComputer::addComponent(const Component & component)
{
    m_Components.emplace_back(component.cloneObject());
    return *this;
}

void CComputer::printNamedObject(ostream & os, bool last, bool firstPipe, bool onePC) const
{
    if (!onePC)
    {
        os << (last ? "\\-Host: "
                    : "+-Host: ")
           << m_Name << '\n';
        for (size_t i = 0; i < m_Addresses.size(); i++)
            os << (firstPipe ? "| "
                             : "  ")
               << (i == m_Addresses.size() - 1
                   && m_Components.empty() ? "\\-"
                                           : "+-")
               << m_Addresses[i] << '\n';
        for (size_t i = 0; i < m_Components.size(); i++)
            m_Components[i]->printComponent(os, (i == m_Components.size() - 1), firstPipe, false);
    }
    else
    {
        os << "Host: " << m_Name << '\n';
        for (size_t i = 0; i < m_Addresses.size(); i++)
            os << (i == m_Addresses.size() - 1
                   && m_Components.empty() ? "\\-"
                                           : "+-")
               << m_Addresses[i] << '\n';
        for (size_t i = 0; i < m_Components.size(); i++)
            m_Components[i]->printComponent(os, (i == m_Components.size() - 1), firstPipe, true);
    }
}

ostream & operator<<(ostream & os, const CComputer & computer)
{
    computer.printNamedObject(os, false, true, true);
    return os;
}
//----------------------------------------------------------------------------------------------------------------------
class CNetwork : public NamedObject
{
public:
    CNetwork(string name) : NamedObject(std::move(name)) {};
    CNetwork(const CNetwork & net);
    CNetwork & operator=(const CNetwork & net);

    CNetwork & addComputer(const NamedObject & computer);
    CComputer * findComputer(const string & computer);
    void printNamedObject(ostream & os, bool last, bool firstPipe, bool onePC) const override;
    NamedObject * cloneObject() const override { return new CNetwork(*this); };

private:
    vector<ANamedObject> m_Computers;
};

CNetwork::CNetwork(const CNetwork & net) : NamedObject(net.m_Name)
{
    for (auto & computer : net.m_Computers)
        addComputer(*computer);
}

CNetwork & CNetwork::operator=(const CNetwork & net)
{
    if (this == &net)
        return *this;

    CNetwork copyToSwap(net);
    swap(m_Name, copyToSwap.m_Name);
    swap(m_Computers, copyToSwap.m_Computers);
    return *this;
}

CNetwork & CNetwork::addComputer(const NamedObject & computer)
{
    m_Computers.emplace_back(computer.cloneObject());
    return *this;
}

CComputer * CNetwork::findComputer(const string & computer)
{
    for (const auto & pc : m_Computers)
        if (pc->getName() == computer)
            return dynamic_cast<CComputer *>(pc.get());
    return nullptr;
}

void CNetwork::printNamedObject(ostream & os, bool last, bool firstPipe, bool onePC) const
{
    os << "Network: " << m_Name << '\n';
    for (size_t i = 0; i < m_Computers.size(); i++)
        m_Computers[i]->printNamedObject(os, (i == m_Computers.size() - 1), i != m_Computers.size() - 1, false);
}
//----------------------------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__
template<typename T_>
std::string toString ( const T_ & x )
{
  std::ostringstream oss;
  oss << x;
  return oss . str ();
}

int main ()
{
  CNetwork n ( "FIT network" );
  n . addComputer (
        CComputer ( "progtest.fit.cvut.cz" ) .
          addAddress ( "147.32.232.142" ) .
          addComponent ( CCPU ( 8, 2400 ) ) .
          addComponent ( CCPU ( 8, 1200 ) ) .
          addComponent ( CDisk ( CDisk::MAGNETIC, 1500 ) .
            addPartition ( 50, "/" ) .
            addPartition ( 5, "/boot" ).
            addPartition ( 1000, "/var" ) ) .
          addComponent ( CDisk ( CDisk::SSD, 60 ) .
            addPartition ( 60, "/data" )  ) .
          addComponent ( CMemory ( 2000 ) ).
          addComponent ( CMemory ( 2000 ) ) ) .
      addComputer (
        CComputer ( "courses.fit.cvut.cz" ) .
          addAddress ( "147.32.232.213" ) .
          addComponent ( CCPU ( 4, 1600 ) ) .
          addComponent ( CMemory ( 4000 ) ).
          addComponent ( CDisk ( CDisk::MAGNETIC, 2000 ) .
            addPartition ( 100, "/" )   .
            addPartition ( 1900, "/data" ) ) ) .
      addComputer (
        CComputer ( "imap.fit.cvut.cz" ) .
          addAddress ( "147.32.232.238" ) .
          addComponent ( CCPU ( 4, 2500 ) ) .
          addAddress ( "2001:718:2:2901::238" ) .
          addComponent ( CMemory ( 8000 ) ) );
  assert ( toString ( n ) ==
    "Network: FIT network\n"
    "+-Host: progtest.fit.cvut.cz\n"
    "| +-147.32.232.142\n"
    "| +-CPU, 8 cores @ 2400MHz\n"
    "| +-CPU, 8 cores @ 1200MHz\n"
    "| +-HDD, 1500 GiB\n"
    "| | +-[0]: 50 GiB, /\n"
    "| | +-[1]: 5 GiB, /boot\n"
    "| | \\-[2]: 1000 GiB, /var\n"
    "| +-SSD, 60 GiB\n"
    "| | \\-[0]: 60 GiB, /data\n"
    "| +-Memory, 2000 MiB\n"
    "| \\-Memory, 2000 MiB\n"
    "+-Host: courses.fit.cvut.cz\n"
    "| +-147.32.232.213\n"
    "| +-CPU, 4 cores @ 1600MHz\n"
    "| +-Memory, 4000 MiB\n"
    "| \\-HDD, 2000 GiB\n"
    "|   +-[0]: 100 GiB, /\n"
    "|   \\-[1]: 1900 GiB, /data\n"
    "\\-Host: imap.fit.cvut.cz\n"
    "  +-147.32.232.238\n"
    "  +-2001:718:2:2901::238\n"
    "  +-CPU, 4 cores @ 2500MHz\n"
    "  \\-Memory, 8000 MiB\n" );
  CNetwork x = n;
  auto c = x . findComputer ( "imap.fit.cvut.cz" );
  assert ( toString ( *c ) ==
    "Host: imap.fit.cvut.cz\n"
    "+-147.32.232.238\n"
    "+-2001:718:2:2901::238\n"
    "+-CPU, 4 cores @ 2500MHz\n"
    "\\-Memory, 8000 MiB\n" );
  c -> addComponent ( CDisk ( CDisk::MAGNETIC, 1000 ) .
         addPartition ( 100, "system" ) .
         addPartition ( 200, "WWW" ) .
         addPartition ( 700, "mail" ) );
  assert ( toString ( x ) ==
    "Network: FIT network\n"
    "+-Host: progtest.fit.cvut.cz\n"
    "| +-147.32.232.142\n"
    "| +-CPU, 8 cores @ 2400MHz\n"
    "| +-CPU, 8 cores @ 1200MHz\n"
    "| +-HDD, 1500 GiB\n"
    "| | +-[0]: 50 GiB, /\n"
    "| | +-[1]: 5 GiB, /boot\n"
    "| | \\-[2]: 1000 GiB, /var\n"
    "| +-SSD, 60 GiB\n"
    "| | \\-[0]: 60 GiB, /data\n"
    "| +-Memory, 2000 MiB\n"
    "| \\-Memory, 2000 MiB\n"
    "+-Host: courses.fit.cvut.cz\n"
    "| +-147.32.232.213\n"
    "| +-CPU, 4 cores @ 1600MHz\n"
    "| +-Memory, 4000 MiB\n"
    "| \\-HDD, 2000 GiB\n"
    "|   +-[0]: 100 GiB, /\n"
    "|   \\-[1]: 1900 GiB, /data\n"
    "\\-Host: imap.fit.cvut.cz\n"
    "  +-147.32.232.238\n"
    "  +-2001:718:2:2901::238\n"
    "  +-CPU, 4 cores @ 2500MHz\n"
    "  +-Memory, 8000 MiB\n"
    "  \\-HDD, 1000 GiB\n"
    "    +-[0]: 100 GiB, system\n"
    "    +-[1]: 200 GiB, WWW\n"
    "    \\-[2]: 700 GiB, mail\n" );
  assert ( toString ( n ) ==
    "Network: FIT network\n"
    "+-Host: progtest.fit.cvut.cz\n"
    "| +-147.32.232.142\n"
    "| +-CPU, 8 cores @ 2400MHz\n"
    "| +-CPU, 8 cores @ 1200MHz\n"
    "| +-HDD, 1500 GiB\n"
    "| | +-[0]: 50 GiB, /\n"
    "| | +-[1]: 5 GiB, /boot\n"
    "| | \\-[2]: 1000 GiB, /var\n"
    "| +-SSD, 60 GiB\n"
    "| | \\-[0]: 60 GiB, /data\n"
    "| +-Memory, 2000 MiB\n"
    "| \\-Memory, 2000 MiB\n"
    "+-Host: courses.fit.cvut.cz\n"
    "| +-147.32.232.213\n"
    "| +-CPU, 4 cores @ 1600MHz\n"
    "| +-Memory, 4000 MiB\n"
    "| \\-HDD, 2000 GiB\n"
    "|   +-[0]: 100 GiB, /\n"
    "|   \\-[1]: 1900 GiB, /data\n"
    "\\-Host: imap.fit.cvut.cz\n"
    "  +-147.32.232.238\n"
    "  +-2001:718:2:2901::238\n"
    "  +-CPU, 4 cores @ 2500MHz\n"
    "  \\-Memory, 8000 MiB\n" );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
