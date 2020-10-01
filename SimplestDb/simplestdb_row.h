// rows should:
// constant time get/set to any field given offset
// serialize/deserialize itself 
// initialize to the correct capacity
// rows should not:
// hold column order

#pragma once
#include"simplestdb_table.h"

#include<vector>
#include<string>
#include<utility>
#include<unordered_map>

//rows are kept on disk in the following order: row id  32 bit | short string pointer | short blob pointer | integers 32 bit | boolean 8 bit
enum class SQLTypes { INTEGER, BOOLEAN, VARCHAR, BLOB };
//name -> type, offset in bytes in serialized row


namespace sdb {

class Row {
 public:
  Row(uint32_t id, Table* table_header, unsigned char* free_start, unsigned char* free_end); // TODO: how to make the pointers const?
  Row(Table* table_header, unsigned char* data_start, unsigned char* data_end);
  ~Row();

  template <typename T>
  T get(std::string);

  //returns false on set failure ie. not enough free space;
  //returns true on success;
  template <typename T>
  bool set(std::string, T);

  void print();

  void pack();

  //move() is called if the new start location != old data start
  void giveSpace(unsigned char* new_free_space_start, unsigned char* new_free_space_end);
  //returns false if the move failed
  bool move(unsigned char* new_location_start, unsigned char* new_location_end);

  size_t size();

 private:
   Table* header;

  //the data is in [data_start, data_end)
  //total space is in [data_start, free_end)
  //free space is in [data_end, free_end)
  unsigned char* data_start;
  unsigned char* data_end;
  unsigned char* free_end;
};

}//namespace sdb