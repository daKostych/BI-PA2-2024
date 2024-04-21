#pragma once

#include "builder.h"
#include <variant>
#include <memory>
#include <map>

constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0x02;
constexpr unsigned SPREADSHEET_FILE_IO = 0x04;
constexpr unsigned SPREADSHEET_SPEED = 0x08;
constexpr unsigned SPREADSHEET_PARSER = 0x10;

using CValue = std::variant<std::monostate, double, std::string>;
using ANode = unique_ptr<Node>;
//======================================================================================================================
class CSpreadsheet
{
public:
    static unsigned capabilities()
    {
        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED;
    }
    CSpreadsheet() = default;
    CSpreadsheet(const CSpreadsheet & other);
    CSpreadsheet(CSpreadsheet && other) noexcept;
    CSpreadsheet & operator=(const CSpreadsheet & other);
    CSpreadsheet & operator=(CSpreadsheet && other) noexcept;

    bool load(std::istream & is);
    static bool loadSell(const string & cell, CSpreadsheet & spreadSheet);
    bool save(std::ostream & os) const;
    bool setCell(CPos pos, std::string contents);
    CValue getValue(CPos pos);
    void copyRect(CPos dst, CPos src, int w = 1, int h = 1);

private:
    map<CPos, ANode> _table;
};
//======================================================================================================================