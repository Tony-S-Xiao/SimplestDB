#include"simplestdb_row.h"

#include<queue>
#include<stdexcept>

sdb::Row::Row(void* start, void* end) {
  start_ptr = static_cast<unsigned char*>(start);
  end_ptr = static_cast<unsigned char*>(end);
}
sdb::Row::~Row() {

}
size_t sdb::Row::physicalSize() {
  return end_ptr - start_ptr;
}

sdb::DBRow::DBRow(void* start, void* end) : Row(start, end) {
  slot_id = static_cast<unsigned int*>(end);
  --slot_id;
  name_end_ptr = reinterpret_cast<unsigned char*>(slot_id);
}
sdb::DBRow::~DBRow() {

}
std::string sdb::DBRow::getTableName() {
  std::string ans{ "" };
  unsigned char* trav = start_ptr;
  while (trav != name_end_ptr) {
    ans.push_back(*trav);
    ++trav;
  }
  return ans;
}
void sdb::DBRow::setTableName(std::string name) {
  unsigned char* trav = start_ptr;
  auto name_it = name.begin();
  while (trav != name_end_ptr && name_it != name.end()) {
    *trav = *name_it;
    ++trav;
    ++name_it;
  }
}
unsigned int sdb::DBRow::getPageId() {
  return *slot_id;
}
void sdb::DBRow::setPageId(unsigned int page_id) {
  *this->slot_id = page_id;
}

sdb::TableHeaderRow::TableHeaderRow(void* start, void* end) :
  Row(start, end),
  num_of_col(static_cast<unsigned short*>(start)),
  start_of_pointers(num_of_col +1)
{

}
sdb::TableHeaderRow::~TableHeaderRow() {

}
unsigned short sdb::TableHeaderRow::getNumOfCol() {
  return *num_of_col;
}
std::string sdb::TableHeaderRow::getColumnName(int i) {
  unsigned short* trav = start_of_pointers + i; 
  char* string_start = reinterpret_cast<char*>(num_of_col) + *trav + 1;
  ++trav;
  char* string_end = reinterpret_cast<char*>(num_of_col) + *trav;
  return std::string(string_start, string_end);
}
sdb::SQLType sdb::TableHeaderRow::getColumnType(int i) {
  unsigned short* trav = start_of_pointers + i;
  char* type_start = reinterpret_cast<char*>(num_of_col) + *trav;
  switch (*type_start) {
  case static_cast<int>(SQLType::NUL):
    return SQLType::NUL;
  case static_cast<int>(SQLType::VARCHAR):
    return SQLType::VARCHAR;
  case static_cast<int>(SQLType::INTEGER):
    return SQLType::INTEGER;
  case static_cast<int>(SQLType::DATETIME):
    return SQLType::DATETIME;
  case static_cast<int>(SQLType::BOOLEAN):
    return SQLType::BOOLEAN;
  }
  return SQLType::NUL;
}
void sdb::TableHeaderRow::appendCol(std::string name, SQLType type) {
  char* data_shift_ptr_start = reinterpret_cast<char*>(num_of_col) + 2 + *num_of_col * 2;
  char* data_shift_ptr_end = reinterpret_cast<char*>(num_of_col) + 2 + *num_of_col * 2 - 2;
  std::queue<char> shift_two;

  //shift data over using buffer
  //buffer allows for 1 pass solution
  while (data_shift_ptr_start != reinterpret_cast<char*>(end_ptr)) {
    if (shift_two.size() < 2) {
      shift_two.push(*data_shift_ptr_start);
    }
    else {
      shift_two.push(*data_shift_ptr_start);
      *data_shift_ptr_end = shift_two.front();
      shift_two.pop();
    }
    ++data_shift_ptr_start;
    ++data_shift_ptr_end;
  }
  while (shift_two.size() > 0) {
    *data_shift_ptr_end = shift_two.front();
    shift_two.pop();
  }

  //update pointers after shift by 2
  unsigned short* to_inc = start_of_pointers;
  for (int i = 0; i < *num_of_col; ++i) {
    *to_inc += 2;
    ++to_inc;
  }

  //append data type
  switch (type) {
  case SQLType::NUL:
    name.push_back(0);
    break;
  case SQLType::VARCHAR:
    name.push_back(1);
    break;
  case SQLType::INTEGER:
    name.push_back(2);
    break;
  case SQLType::DATETIME:
    name.push_back(3);
    break;
  case SQLType::BOOLEAN:
    name.push_back(4);
    break;
  }

  //initialize write pointer
  unsigned short* append_index = start_of_pointers + *num_of_col;
  char* write_ptr = reinterpret_cast<char*>(num_of_col) + *(append_index - 1) + 1;
  ++write_ptr;
  //write data
  for (char c : name) {
    if (write_ptr < reinterpret_cast<char*>(end_ptr))
    {
    *write_ptr = c;
    ++write_ptr;
    }
  }
  //update pointer
  *append_index = static_cast<unsigned short>(write_ptr - reinterpret_cast<char*>(start_ptr));

  //update number of total col
  ++*num_of_col;

  return;
}

sdb::TablePointerRow::TablePointerRow(void* start, void* end) : Row(start, end) {
  page_id = static_cast<unsigned int*>(start);
  space_available_on_page = reinterpret_cast<unsigned short*>(page_id + 1);
}
sdb::TablePointerRow::~TablePointerRow() {

}
void sdb::TablePointerRow::setPageId(unsigned int index) {
  *page_id = index;
}
void sdb::TablePointerRow::setSpaceAvailable(unsigned short space) {
  *space_available_on_page = space;
}
unsigned int sdb::TablePointerRow::getPageId() {
  return *page_id;
}
unsigned short sdb::TablePointerRow::getSpaceAvailable() {
  return *space_available_on_page;
}

sdb::DataRow::DataRow(void* start, void* end) : Row(start, end) {
  flag_ptr = static_cast<char*>(start);
}

sdb::DataRow::~DataRow() {

}

int sdb::DataRow::getInteger(int i) {

}
bool getBoolean(int i);
std::string getVarChar(int i);
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
  switch (type) {
  case SQLType::DATETIME:
    helpSetBit(3, true);
    return;
  case SQLType::INTEGER:
    helpSetBit(2, true);
    return;
  case SQLType::BOOLEAN:
    helpSetBit(1, true);
    return;
  case SQLType::VARCHAR:
    helpSetBit(0, true);
    return;
  }
  throw std::runtime_error("SQL type not implemented.");
}
void sdb::DataRow::unsetFlagBit(SQLType type) {
  switch (type) {
  case SQLType::DATETIME:
    helpSetBit(3, false);
    return;
  case SQLType::INTEGER:
    helpSetBit(2, false);
    return;
  case SQLType::BOOLEAN:
    helpSetBit(1, false);
    return;
  case SQLType::VARCHAR:
    helpSetBit(0, false);
    return;
  }
  throw std::runtime_error("SQL type not implemented.");
}
bool sdb::DataRow::getFlagBit(SQLType type) {
  switch (type) {
  case SQLType::DATETIME:
    return helpGetBit(3);
  case SQLType::INTEGER:
    return helpGetBit(2);
  case SQLType::BOOLEAN:
    return helpGetBit(1);
  case SQLType::VARCHAR:
    return helpGetBit(0);
  }
  throw std::runtime_error("SQL type not implemented.");
}