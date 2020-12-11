#include"simplestdb_datarow.h"
#include"simplestdb_util.h"
#include"simplestdb_token.h"

#include<queue>
#include<stack>
#include<stdexcept>
#include<cstring>
#include<vector>
#include<math.h>

sdb::DataRow::DataRow(std::byte* begin, std::byte* end) :
  Row(begin, end),
  flag_ptr_(reinterpret_cast<unsigned char*>(begin)) {
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
  uint16_t* offsets{ reinterpret_cast<uint16_t*>(flag_ptr_ + 1) };
  std::byte* trav = reinterpret_cast<std::byte*>(flag_ptr_ + 1);
  for (int i = 0; i < to_update.size(); ++i) {
    start_address.push(trav + *offsets);
    end_address.push(trav + *offsets);
    trav += sizeof(uint16_t);
    ++offsets;
  }
  while (to_update.size() > 0) {
    switch (to_update.front()) {
    case SQLType::DATETIME:
      datetime_begin_ptr_ = reinterpret_cast<char*>(start_address.front());
      datetime_end_ptr_ = reinterpret_cast<char*>(end_address.front());
      break;
    case SQLType::INTEGER:
      int_begin_ptr_ = reinterpret_cast<int32_t*>(start_address.front());
      int_end_ptr_ = reinterpret_cast<int32_t*>(end_address.front());
      break;
    case SQLType::BOOLEAN:
      bool_begin_ptr_ = reinterpret_cast<bool8_t*>(start_address.front());
      bool_end_ptr_ = reinterpret_cast<bool8_t*>(end_address.front());
      break;
    }
    to_update.pop();
    start_address.pop();
    end_address.pop();
  }
  if (getFlagBit(SQLType::VARCHAR))
    varchar_begin_ptr_ = reinterpret_cast<uint16_t*>(start_address.front());
}
int sdb::DataRow::getInteger(int i) {
  if (getFlagBit(SQLType::INTEGER) && int_begin_ptr_ + i < int_end_ptr_) {
    return *(int_begin_ptr_ + i);
  }
  throw std::runtime_error("Type mismatch or out-of-range getting data.");
}
bool sdb::DataRow::getBoolean(int i) {
  if (getFlagBit(SQLType::BOOLEAN) && bool_begin_ptr_ + i < bool_end_ptr_) {
    return *(bool_begin_ptr_ + i);
  }
  throw std::runtime_error("Type mismatch or out-of-range getting data.");
}
std::string sdb::DataRow::getVarChar(int i) {
  if (getFlagBit(SQLType::VARCHAR)) {
    char* trav = static_cast<char*>(std::max({ static_cast<void*>(datetime_end_ptr_),
                static_cast<void*>(int_end_ptr_), static_cast<void*>(bool_end_ptr_) }));
    for (int j = 0; j < i; ++j) {
      if (*trav == 0)   throw std::runtime_error("Type mismatch or out-of-range getting data.");
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
  // Populate block from the left.
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
  // Copy data and increment write pointer and set the pos.
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
  // Copy the contents of serialized string.
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
  string_serialized_size += sizeof(unsigned short); // Null unsigned short terminated.
  return sizeof(*flag_ptr_) +
    (all_bool.size() > 0 ? sizeof(OnRowPointer) : 0) +
    (all_integer.size() > 0 ? sizeof(OnRowPointer) : 0) +
    (all_datetime.size() > 0 ? sizeof(OnRowPointer) : 0) +
    all_bool.size() * sizeof(bool8_t) +
    all_integer.size() * sizeof(int) +
    all_datetime.size() * sizeof(char) +
    string_serialized_size;
}
size_t sdb::DataRow::getHeaderSize() {
  return 1 + sizeof(OnRowPointer) * (getFlagBit(SQLType::DATETIME) + getFlagBit(SQLType::INTEGER) + getFlagBit(SQLType::BOOLEAN));
}
void sdb::DataRow::setFlagBit(SQLType type) {
  *flag_ptr_ |= static_cast<unsigned char>(type);
}
void sdb::DataRow::unsetFlagBit(SQLType type) {
  *flag_ptr_ ^= static_cast<unsigned char>(type);
}
bool sdb::DataRow::getFlagBit(SQLType type) {
  return static_cast<unsigned char>(type) & *flag_ptr_;
}
bool sdb::DataRow::flagIsNull() {
  return *flag_ptr_ == 0;
}