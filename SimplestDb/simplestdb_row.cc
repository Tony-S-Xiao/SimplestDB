#include"simplestdb_row.h"
#include"simplestdb_util.h"

#include<queue>
#include<stack>
#include<stdexcept>
#include<cstring>
#include<vector>
#include<math.h>

sdb::Row::Row(std::byte* begin, std::byte* end) :
  row_begin_(begin), row_end_(end) {
}
size_t sdb::Row::physicalSize() {
  return row_end_ - row_begin_;
}
/*****/
sdb::DBRow::DBRow(std::byte* begin, std::byte* end) : Row(begin, end) {
  page_id_ = reinterpret_cast<OnDiskPointer*>(end) - 1;
  tablename_end_ = reinterpret_cast<std::byte*>(page_id_);
}
std::string sdb::DBRow::getTableName() {
  return std::string(reinterpret_cast<char*>(row_begin_), tablename_end_ - row_begin_);
}
sdb::OnDiskPointer sdb::DBRow::getPageId() {
  return *page_id_;
}
void sdb::DBRow::setTableName(const std::string& name) {
  memcpy(reinterpret_cast<char*>(row_begin_), name.data(), std::min(name.size(), row_end_ - row_begin_ - sizeof(uint32_t)));
}
void sdb::DBRow::setPageId(uint32_t page_id) {
  *page_id_ = page_id;
}
size_t sdb::DBRow::calcSizeRequired(const std::string& name) {
  return name.size() + sizeof(unsigned int);
}
/*****/
sdb::TableHeaderRow::TableHeaderRow(std::byte* begin, std::byte* end) :
  Row(begin, end),
  num_of_col_(reinterpret_cast<uint16_t*>(begin)),
  start_of_pointers_(num_of_col_ +1) {
}
unsigned short sdb::TableHeaderRow::getNumOfCol() {
  return *num_of_col_;
}
std::string sdb::TableHeaderRow::getColumnName(int i) {
  unsigned short* trav = reinterpret_cast<unsigned short*>(start_of_pointers_) + i;
  char* string_start{ nullptr };
  char* string_end = reinterpret_cast<char*>(trav) + *trav;
  if (i == 0)
    string_start = reinterpret_cast<char*>(row_begin_) + sizeof(uint16_t) + sizeof(uint16_t) * getNumOfCol();
  else {
    --trav;
    string_start = reinterpret_cast<char*>(trav) + *trav;
  }
  return std::string(string_start, string_end - string_start);
}
sdb::SQLType sdb::TableHeaderRow::getColumnType(int i) {
  unsigned short* trav = reinterpret_cast<unsigned short*>(start_of_pointers_) + i;
  char* type_start = reinterpret_cast<char*>(num_of_col_) + *trav;
  switch (*type_start) {
  case static_cast<unsigned char>(SQLType::NUL):
    return SQLType::NUL;
  case static_cast<unsigned char>(SQLType::VARCHAR):
    return SQLType::VARCHAR;
  case static_cast<unsigned char>(SQLType::INTEGER):
    return SQLType::INTEGER;
  case static_cast<unsigned char>(SQLType::DATETIME):
    return SQLType::DATETIME;
  case static_cast<unsigned char>(SQLType::BOOLEAN):
    return SQLType::BOOLEAN;
  }
  return SQLType::NUL;
}
//void sdb::TableHeaderRow::appendCol(std::string name, SQLType type) {
//  char* data_shift_ptr_start = reinterpret_cast<char*>(num_of_col) + 2 + *num_of_col * 2;
//  char* data_shift_ptr_end = reinterpret_cast<char*>(num_of_col) + 2 + *num_of_col * 2 - 2;
//  std::queue<char> shift_two;
//
//  //shift data over using buffer
//  //buffer allows for 1 pass solution
//  while (data_shift_ptr_start != reinterpret_cast<char*>(row_end_)) {
//    if (shift_two.size() < 2) {
//      shift_two.push(*data_shift_ptr_start);
//    }
//    else {
//      shift_two.push(*data_shift_ptr_start);
//      *data_shift_ptr_end = shift_two.front();
//      shift_two.pop();
//    }
//    ++data_shift_ptr_start;
//    ++data_shift_ptr_end;
//  }
//  while (shift_two.size() > 0) {
//    *data_shift_ptr_end = shift_two.front();
//    shift_two.pop();
//  }
//
//  //update pointers after shift by 2
//  unsigned short* to_inc = start_of_pointers;
//  for (int i = 0; i < *num_of_col; ++i) {
//    *to_inc += 2;
//    ++to_inc;
//  }
//
//  //append data type
//  switch (type) {
//  case SQLType::NUL:
//    name.push_back(0);
//    break;
//  case SQLType::VARCHAR:
//    name.push_back(1);
//    break;
//  case SQLType::INTEGER:
//    name.push_back(2);
//    break;
//  case SQLType::DATETIME:
//    name.push_back(3);
//    break;
//  case SQLType::BOOLEAN:
//    name.push_back(4);
//    break;
//  }
//
//  //initialize write pointer
//  unsigned short* append_index = start_of_pointers + *num_of_col;
//  char* write_ptr = reinterpret_cast<char*>(num_of_col) + *(append_index - 1) + 1;
//  ++write_ptr;
//  //write data
//  for (char c : name) {
//    if (write_ptr < reinterpret_cast<char*>(row_end_))
//    {
//    *write_ptr = c;
//    ++write_ptr;
//    }
//  }
//  //update pointer
//  *append_index = static_cast<unsigned short>(write_ptr - reinterpret_cast<char*>(row_begin_));
//
//  //update number of total col
//  ++*num_of_col;
//
//  return;
//}
void sdb::TableHeaderRow::loadData(const std::vector<std::string>& all_column_names,
  const std::vector<sdb::SQLType>& all_types) {
  uint16_t* write_ptr_16{ reinterpret_cast<uint16_t*>(row_begin_) };
  unsigned char* write_ptr_8{ reinterpret_cast<uint8_t*>(row_begin_) };
  std::vector<std::byte> byte_buffer;
  std::vector<OnRowPointer> pointer_buffer;
  /* Flatten column names and append with their type */
  for (int i = 0; i < all_column_names.size(); ++i) {
    for (char curr_char : all_column_names[i]) {
      byte_buffer.push_back(static_cast<std::byte>(curr_char));
    }
    byte_buffer.push_back(static_cast<std::byte>(all_types[i]));
    pointer_buffer.push_back(static_cast<OnRowPointer>(byte_buffer.size()));
  }
  /* Pointers needs additional offset */
  for (int i = 0; i < pointer_buffer.size(); ++i) {
    pointer_buffer[i] += pointer_buffer.size() - 1 - i;
  }
  *num_of_col_ = static_cast<uint16_t>(all_column_names.size());
  memcpy(start_of_pointers_, &pointer_buffer, pointer_buffer.size() * sizeof(OnRowPointer));
  memcpy(start_of_pointers_ + pointer_buffer.size(), &byte_buffer, byte_buffer.size());
}
size_t sdb::TableHeaderRow::calcSizeRequired(const std::vector<std::string>& all_column_names,
  const std::vector<SQLType>& all_types) {
  size_t size_in_bytes{ sizeof(uint16_t) + sizeof(uint16_t) * all_column_names.size() };
  for (int i = 0; i < all_column_names.size(); ++i) {
    size_in_bytes += all_column_names[i].size() + sizeof(unsigned char);
  }
  return size_in_bytes;
}
/*****/
sdb::TablePointerRow::TablePointerRow(std::byte* begin, std::byte* end) : Row(begin, end) {
  page_id_ = reinterpret_cast<unsigned int*>(begin);
  space_available_on_page_ = reinterpret_cast<unsigned short*>(page_id_ + 1);
}
void sdb::TablePointerRow::setPageId(unsigned int index) {
  *page_id_ = index;
}
void sdb::TablePointerRow::setSpaceAvailable(unsigned short space) {
  *space_available_on_page_ = space;
}
unsigned int sdb::TablePointerRow::getPageId() {
  return *page_id_;
}
unsigned short sdb::TablePointerRow::getSpaceAvailable() {
  return *space_available_on_page_;
}
/*****/
sdb::DataRow::DataRow(std::byte* begin, std::byte* end) :
  Row(begin, end),
  flag_ptr(reinterpret_cast<unsigned char*>(begin)) {
  init();
}

void sdb::DataRow::init() {
  std::queue<std::byte*> start_address;
  std::queue<std::byte*> end_address;
  std::queue<SQLType> to_update;
  start_address.push(row_begin_ + getHeaderSize());
  if (getFlagBit(SQLType::DATETIME)) {
    to_update.push(SQLType::DATETIME);
  }
  if (getFlagBit(SQLType::INTEGER)) {
    to_update.push(SQLType::INTEGER);
  }
  if (getFlagBit(SQLType::BOOLEAN)) {
    to_update.push(SQLType::BOOLEAN);
  }
  uint16_t* offsets{ reinterpret_cast<uint16_t*>(flag_ptr + 1) };
  std::byte* trav = reinterpret_cast<std::byte*>(flag_ptr + 1);
  for (int i = 0; i < to_update.size(); ++i) {
    start_address.push(trav + *offsets);
    end_address.push(trav + *offsets);
    trav += sizeof(uint16_t);
    ++offsets;
  }
  while (to_update.size() > 0) {
    switch (to_update.front()) {
    case SQLType::DATETIME:
      datetime_begin_ptr = reinterpret_cast<char*>(start_address.front());
      datetime_end_ptr = reinterpret_cast<char*>(end_address.front());
      break;
    case SQLType::INTEGER:
      int_begin_ptr = reinterpret_cast<int32_t*>(start_address.front());
      int_end_ptr = reinterpret_cast<int32_t*>(end_address.front());
      break;
    case SQLType::BOOLEAN:
      bool_begin_ptr = reinterpret_cast<bool8_t*>(start_address.front());
      bool_end_ptr = reinterpret_cast<bool8_t*>(end_address.front());
      break;
    }
    to_update.pop();
    start_address.pop();
    end_address.pop();
  }
  if (getFlagBit(SQLType::VARCHAR))
    varchar_begin_ptr = reinterpret_cast<uint16_t*>(start_address.front());
}

int sdb::DataRow::getInteger(int i) {
  if (getFlagBit(SQLType::INTEGER) && int_begin_ptr + i < int_end_ptr) {
    return *(int_begin_ptr + i);
  }
  throw std::runtime_error("Type mismatch or out-of-range getting data.");
}
bool sdb::DataRow::getBoolean(int i) {
  if (getFlagBit(SQLType::BOOLEAN) && bool_begin_ptr + i < bool_end_ptr) {
    return *(bool_begin_ptr + i);
  }
  throw std::runtime_error("Type mismatch or out-of-range getting data.");
}
std::string sdb::DataRow::getVarChar(int i) {
  if (getFlagBit(SQLType::VARCHAR)) {
    char* trav = static_cast<char*>(std::max({static_cast<void*>(datetime_end_ptr),
                static_cast<void*>(int_end_ptr), static_cast<void*>(bool_end_ptr)}));
    for (int j = 0; j < i; ++j) {
      if(*trav == 0)   throw std::runtime_error("Type mismatch or out-of-range getting data.");
      trav += *reinterpret_cast<unsigned short*>(trav);
    }
    char* start_of_result_string = reinterpret_cast<char*>(trav) + sizeof(unsigned short);
    trav += *reinterpret_cast<unsigned short*>(trav);
    char* end_of_result_string = reinterpret_cast<char*>(trav);
    return std::string(start_of_result_string, end_of_result_string - start_of_result_string);
  }
  throw std::runtime_error("Type mismatch or out-of-range getting data.");
}
void sdb::DataRow::loadData(const std::vector<bool8_t>& all_bool, const std::vector<int>& all_integer,
  const std::vector<std::string>& all_datetime, const std::vector<std::string>& all_string) {
  unsigned char* write_pos_ptr{ reinterpret_cast<unsigned char*>(row_begin_) };
  ++write_pos_ptr;
  unsigned short* pos_to_datetime{ nullptr };
  unsigned short* pos_to_integer{ nullptr };
  unsigned short* pos_to_boolean{ nullptr };
  unsigned short* pos_to_varchar{ nullptr };
  /*
  ** populate block from the left
  */
  if (all_datetime.size() > 0) {
    setFlagBit(SQLType::DATETIME);
    pos_to_datetime = reinterpret_cast<unsigned short*>(write_pos_ptr);
    write_pos_ptr += sizeof(unsigned short);
  }
  if (all_integer.size() > 0) {
    setFlagBit(SQLType::INTEGER);
    pos_to_integer = reinterpret_cast<unsigned short*>(write_pos_ptr);
    write_pos_ptr += sizeof(unsigned short);
  }
  if (all_bool.size() > 0) {
    setFlagBit(SQLType::BOOLEAN);
    pos_to_boolean = reinterpret_cast<unsigned short*>(write_pos_ptr);
    write_pos_ptr += sizeof(unsigned short);
  }
  /*
  ** copy data and increment write pointer and set the pos
  */
  if (all_datetime.size() > 0) {
    std::memcpy(write_pos_ptr, all_datetime.data(), all_datetime.size() * sizeof(char));
    write_pos_ptr += all_datetime.size() * sizeof(char);
    *pos_to_datetime = static_cast<unsigned int>(write_pos_ptr - reinterpret_cast<unsigned char*>(pos_to_datetime));
  }
  if (all_integer.size() > 0) {
    std::memcpy(write_pos_ptr, all_integer.data(), all_integer.size() * sizeof(int));
    write_pos_ptr += all_integer.size() * sizeof(int);
    *pos_to_integer = static_cast<unsigned int>(write_pos_ptr - reinterpret_cast<unsigned char*>(pos_to_integer));
  }
  if (all_bool.size() > 0) {
    std::memcpy(write_pos_ptr, all_bool.data(), all_bool.size() * sizeof(char));
    write_pos_ptr += all_bool.size() * sizeof(char);
    *pos_to_boolean = static_cast<unsigned int>(write_pos_ptr - reinterpret_cast<unsigned char*>(pos_to_boolean));
  }
  /* Copy the contents of serialized string
  */
  if (all_string.size() > 0) {
    setFlagBit(SQLType::VARCHAR);
    std::string serialized_string{ sdb::serializeStringVec(all_string) };
    std::memcpy(write_pos_ptr, serialized_string.data(), serialized_string.size() * sizeof(char));
  }
  init();
}
size_t sdb::DataRow::calcSizeRequired(const std::vector<bool8_t>& all_bool, const std::vector<int>& all_integer,
  const std::vector<std::string>& all_datetime, const std::vector<std::string>& all_string) {
  size_t string_serialized_size{ 0 };
  for (const std::string& s : all_string) {
    string_serialized_size += s.size() + sizeof(unsigned short);
  }
  string_serialized_size += 2; // Null unsigned short terminated.
  return sizeof(*flag_ptr) +
    (all_bool.size() > 0 ? sizeof(OnRowPointer) : 0) +
    (all_integer.size() > 0 ? sizeof(OnRowPointer) : 0) +
    (all_datetime.size() > 0 ? sizeof(OnRowPointer) : 0) +
    all_bool.size() * sizeof(bool8_t) +
    all_integer.size() * sizeof(int) +
    all_datetime.size() * sizeof(char) +
    string_serialized_size;
}
int sdb::DataRow::getHeaderSize() {
  return 1 + sizeof(OnRowPointer) * (getFlagBit(SQLType::DATETIME) + getFlagBit(SQLType::INTEGER) + getFlagBit(SQLType::BOOLEAN));
}

void sdb::DataRow::helpSetBit(int i, bool set) {
  char mask = 0b00000001;
  for (int j = 0; j < i; ++j) {
    mask <<= 1;
  }
  if (set)
    *flag_ptr = *flag_ptr | mask;
  else
    *flag_ptr = *flag_ptr & ~mask;
}
bool sdb::DataRow::helpGetBit(int i) {
  char mask = 0b00000001;
  for (int j = 0; j < i; ++j) {
    mask <<= 1;
  }
  return *flag_ptr & mask > 0;
}

void sdb::DataRow::setFlagBit(SQLType type) {
  *flag_ptr |= static_cast<unsigned char>(type);
  //switch (type) {
  //case SQLType::DATETIME:
  //  helpSetBit(3, true);
  //  return;
  //case SQLType::INTEGER:
  //  helpSetBit(2, true);
  //  return;
  //case SQLType::BOOLEAN:
  //  helpSetBit(1, true);
  //  return;
  //case SQLType::VARCHAR:
  //  helpSetBit(0, true);
  //  return;
  //}

}
void sdb::DataRow::unsetFlagBit(SQLType type) {
  *flag_ptr ^= static_cast<unsigned char>(type);
  //switch (type) {
  //case SQLType::DATETIME:
  //  helpSetBit(3, false);
  //  return;
  //case SQLType::INTEGER:
  //  helpSetBit(2, false);
  //  return;
  //case SQLType::BOOLEAN:
  //  helpSetBit(1, false);
  //  return;
  //case SQLType::VARCHAR:
  //  helpSetBit(0, false);
  //  return;
  //}

}
bool sdb::DataRow::getFlagBit(SQLType type) {
  return static_cast<unsigned char>(type) & *flag_ptr;
  //switch (type) {
  //case SQLType::DATETIME:
  //  return helpGetBit(3);
  //case SQLType::INTEGER:
  //  return helpGetBit(2);
  //case SQLType::BOOLEAN:
  //  return helpGetBit(1);
  //case SQLType::VARCHAR:
  //  return helpGetBit(0);
  //}

}

bool sdb::DataRow::flagIsNull() {
  return *flag_ptr == 0;
}