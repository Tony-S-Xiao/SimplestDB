#ifndef SIMPLESTDB_DATAROW_H_
#define SIMPLESTDB_DATAROW_H_
#include"simplestdb_row.h"
#include"simplestdb_token.h"

#include<string>
#include<vector>

namespace sdb {
// Contains the data of the table itself(records).
class DataRow : public Row {
public:
  DataRow(std::byte*, std::byte*);
  // Gets the i-th integer, boolean, or string.
  // Index i is independent for each type. Eg. 0-th int != 0-th bool
  // Throws exception if the data at i is not correct type.
  // Throws exception if i is out of range.
  int getInteger(int i);
  bool getBoolean(int i);
  std::string getVarChar(int i);
  // Quickly loads data into a block on a Page.
  // Writes the vectors of data into the block using pointers and memcpy.
  // Booleans are 1 byte chars to avoid using std::vector<bool> specialization. See
  // type alias in namespace sdb in simplest_row.h.
  void loadData(const std::vector<bool8_t>& all_bool, const std::vector<int>& all_integer,
    const std::vector<std::string>& all_datetime, const std::vector<std::string>& all_string);
  static size_t calcSizeRequired(const std::vector<bool8_t>& all_bool, const std::vector<int>& all_integer,
    const std::vector<std::string>& all_datetime, const std::vector<std::string>& all_string);
private:
  // Used to manipulate the first byte of the row(flag byte).
  unsigned char* flag_ptr_{ nullptr };
  void setFlagBit(SQLType type);
  void unsetFlagBit(SQLType type);
  bool getFlagBit(SQLType type);
  bool flagIsNull();
  // Used to initialized data pointers.
  void init();
  // Ranges of the data on the row. [begin, end).
  char* datetime_begin_ptr_{ nullptr };
  char* datetime_end_ptr_{ nullptr };
  int32_t* int_begin_ptr_{ nullptr };
  int32_t* int_end_ptr_{ nullptr };
  bool8_t* bool_begin_ptr_{ nullptr };
  bool8_t* bool_end_ptr_{ nullptr };
  uint16_t* varchar_begin_ptr_{ nullptr };
  size_t getHeaderSize();
};
}  // namespace sdb
#endif // !SIMPLESTDB_DATAROW_H_
