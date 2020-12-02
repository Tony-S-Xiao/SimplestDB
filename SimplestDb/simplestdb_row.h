#pragma once
#include"simplestdb_token.h"

#include<string>

/*
** These objects 
**
**
*/

namespace sdb {

//base class to be derived
class Row {
public:
  ~Row();
  //returns the size in bytes of the row
  size_t physicalSize();
protected:
  //class is abstract effectively 
  Row(void*, void*);
  //start of the data range
  unsigned char* start_ptr;
  //one past the end of the data range
  unsigned char* end_ptr;
};

//this object implements the db object to be instantiated on the first page of the .sdb file
//holds names of tables, page id pairs

class DBRow : public Row {
public:
  DBRow(void*, void*);
  ~DBRow();
  std::string getTableName();
  void setTableName(std::string name);
  unsigned int getPageId();
  void setPageId(unsigned int slot_id = 0);
private:
  unsigned int* slot_id;
  unsigned char* name_end_ptr;
};

//this object contains the table header information for the table
//contains column name, type information
//together with TablePointerRow makes up table pages

class TableHeaderRow : public Row {
public:
  TableHeaderRow(void*, void*);
  ~TableHeaderRow();
  unsigned short getNumOfCol();
  std::string getColumnName(int i);
  SQLType getColumnType(int i);
  void appendCol(std::string name, SQLType type);
private:
  unsigned short* num_of_col;
  unsigned short* start_of_pointers;
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
  using bool8_t = char;  /* Booleans are 1 byte */
  DataRow(void*, void*);
  ~DataRow();
  //std::string getDateTime(int i); TODO: use std chronos

  /* Gets the column at index i in formatted row.
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

}//sdb namespace