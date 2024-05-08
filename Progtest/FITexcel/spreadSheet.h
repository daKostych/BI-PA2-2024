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
    // Static function to return supported capabilities
    static unsigned capabilities()
    {
        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED;
    }

    // Constructors and assignment operators
    CSpreadsheet() = default;                                 // Default constructor
    CSpreadsheet(const CSpreadsheet & other);                 // Copy constructor
    CSpreadsheet(CSpreadsheet && other) noexcept;             // Move constructor
    CSpreadsheet & operator=(const CSpreadsheet & other);     // Copy assignment operator
    CSpreadsheet & operator=(CSpreadsheet && other) noexcept; // Move assignment operator

    // Load spreadsheet from input stream
    bool load(std::istream & is);

    // Static method to load spreadsheet from a specified cell
    static bool loadSell(const string & cell, CSpreadsheet & spreadSheet);

    // Save spreadsheet to output stream
    bool save(std::ostream & os) const;

    // Set the contents of a cell at the specified position
    bool setCell(CPos pos, std::string contents);

    // Get the value stored in the cell at the specified position
    CValue getValue(CPos pos);

    // Copy a rectangular area of cells from source to destination
    void copyRect(CPos dst, CPos src, int w = 1, int h = 1);

private:
    std::map<CPos, ANode> _table; // Internal representation of the spreadsheet
};
//======================================================================================================================