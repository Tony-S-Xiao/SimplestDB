#include"simplestdb_row.h"
#include"simplestdb_token.h"
#include"simplestdb_tableheaderrow.h"

#include<cstddef>
#include<cstdint>
#include<vector>
#include<string>
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
    string_start = reinterpret_cast<char*>(trav) + *trav + 1;
  }
  return std::string(string_start, string_end - string_start);
}
sdb::SQLType sdb::TableHeaderRow::getColumnType(int i) {
  unsigned short* trav = reinterpret_cast<unsigned short*>(start_of_pointers_) + i;
  char* type_start = reinterpret_cast<char*>(trav) + *trav;
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
void sdb::TableHeaderRow::loadData(const std::vector<std::string>& all_column_names,
  const std::vector<sdb::SQLType>& all_types) {
  uint16_t* write_ptr_16{ reinterpret_cast<uint16_t*>(row_begin_) };
  unsigned char* write_ptr_8{ reinterpret_cast<uint8_t*>(row_begin_) };
  std::vector<std::byte> byte_buffer;
  std::vector<OnRowPointer> pointer_buffer;
  // Flatten column names and append with their type.
  for (int i = 0; i < all_column_names.size(); ++i) {
    for (char curr_char : all_column_names[i]) {
      byte_buffer.push_back(static_cast<std::byte>(curr_char));
    }
    byte_buffer.push_back(static_cast<std::byte>(all_types[i]));
    pointer_buffer.push_back(static_cast<OnRowPointer>(byte_buffer.size()));
  }
  // Pointers needs additional offset.
  for (int i = 0; i < pointer_buffer.size(); ++i) {
    pointer_buffer[i] += pointer_buffer.size() * sizeof(OnRowPointer) - i * sizeof(OnRowPointer) - 1;
  }
  *num_of_col_ = static_cast<uint16_t>(all_column_names.size());
  memcpy(start_of_pointers_, pointer_buffer.data(), pointer_buffer.size() * sizeof(OnRowPointer) );
  memcpy(reinterpret_cast<char*>(start_of_pointers_) + pointer_buffer.size() * sizeof(OnRowPointer), byte_buffer.data(), byte_buffer.size());
}
size_t sdb::TableHeaderRow::calcSizeRequired(const std::vector<std::string>& all_column_names,
  const std::vector<SQLType>& all_types) {
  size_t size_in_bytes{ sizeof(uint16_t) + sizeof(uint16_t) * all_column_names.size() };
  for (int i = 0; i < all_column_names.size(); ++i) {
    size_in_bytes += all_column_names[i].size() + sizeof(unsigned char);
  }
  return size_in_bytes;
}