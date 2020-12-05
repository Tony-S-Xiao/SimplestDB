#pragma once
#include"simplestdb_token.h"

#include<string>

// These objects interpret data blocks on Slottedpage objects using pointers.
// All data reside on an array of unsigned chars(see Slottedpage class).
// These can act both like records in tables or on-disk-pointers used in 
// .sdb files or metadata for the database.

namespace sdb {
// Booleans are 1 byte to avoid vector<bool> specialization.
using bool8_t = char;
// Pointers in the rows are unsigned shorts(in number of bytes) offset from where the pointer starts.
// Rows cannot span more than 1 page. So max pointer value only needs to be 2^16. Same as page size.
using OnRowPointer = uint16_t;
// On disk pointers(page id) are the index of page in .sdb file 0th indexed.
using OnDiskPointer = uint32_t;

// Base class to be derived.
class Row {
 public:
  // Number of bytes of the row.
  size_t physicalSize();

 protected:
  // Only used by derived classes. Requires [begin, end)
  Row(std::byte* begin, std::byte* end);
  std::byte* row_begin_;
  std::byte* row_end_;
};

// Contains the table name and an on-disk pointer(page id) to 
// the table.
class DBRow : public Row {
 public:
  DBRow(std::byte* begin, std::byte* end);
  std::string getTableName();
  OnDiskPointer getPageId();
  void setTableName(const std::string& name);
  void setPageId(uint32_t page_id);
  static size_t calcSizeRequired(const std::string& name);
 private:
  OnDiskPointer* page_id_;
  std::byte* tablename_end_;
};

// Contains the header/column name and type information for the table. 
// Acts as an array of column name, column type pairs.
class TableHeaderRow : public Row {
 public:
  TableHeaderRow(std::byte*, std::byte*);
  unsigned short getNumOfCol();
  std::string getColumnName(int i);
  SQLType getColumnType(int i);
  void loadData(const std::vector<std::string>& all_column_names,
    const std::vector<SQLType>& all_types);
  static size_t calcSizeRequired(const std::vector<std::string>& all_column_names,
    const std::vector<SQLType>& all_types);
  //void appendCol(std::string name, SQLType type);
 private:
  uint16_t* num_of_col_;
  uint16_t* start_of_pointers_;
};

//this object contains the space available on the page
//implements the table directory
//contains page id, space available pairs
//together with TableHeaderRows makes up table pages
class TablePointerRow : public Row {
public:
  TablePointerRow(void*, void*);
  ~TablePointerRow();
  void setPageId(unsigned int index);
  void setSpaceAvailable(unsigned short space);
  unsigned int getPageId();
  unsigned short getSpaceAvailable();
private:
  unsigned short* space_available_on_page;
  unsigned int* page_id;
};

//this object contains the data of the table itself
class DataRow : public Row {
public:

  DataRow(void*, void*);
  ~DataRow();
  //std::string getDateTime(int i); TODO: use std chronos
  /* Gets the i-th integer, boolean, or string.
  ** Index i is independent for each type. Eg. 0-th int != 0-th bool
  ** Throws exception if the data at i is not correct type.
  ** Throws exception if i is out of range.
  */
  int getInteger(int i);
  bool getBoolean(int i);
  std::string getVarChar(int i);
  /* Quickly loads data into a block on a Page.
  ** Writes the vectors of data into the block using pointers and memcpy.
  ** Booleans are 1 byte chars to avoid using std::vector<bool> specialization. See type alias in
  ** class declaration.
  */
  void loadData(const std::vector<bool8_t>& all_bool, const std::vector<int>& all_integer,
    const std::vector<std::string>& all_datetime, const std::vector<std::string>& all_string);
private:
  //used to manipulate the first byte of the row(flag byte)
  unsigned char* flag_ptr{ nullptr };
  char* datetime_begin_ptr{ nullptr };
  char* datetime_end_ptr{ nullptr };
  int* int_begin_ptr{ nullptr };
  int* int_end_ptr{ nullptr };
  bool8_t* bool_begin_ptr{ nullptr };
  bool8_t* bool_end_ptr{ nullptr };


  int getHeaderSize();

  void helpSetBit(int i, bool set);
  bool helpGetBit(int i);
  void setFlagBit(SQLType type);
  void unsetFlagBit(SQLType type);
  bool getFlagBit(SQLType type);

};

}  // namespace sdb