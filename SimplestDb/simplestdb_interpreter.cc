#include"simplestdb_interpreter.h"
#include"simplestdb_token.h"
#include"simplestdb_enum_operation.h"
#include"simplestdb_disk_manager.h"
#include"simplestdb_dbrow.h"
#include"simplestdb_tableheaderrow.h"
#include"simplestdb_datarow.h"
#include"simplestdb_tablepointerrow.h"
#include<algorithm>

#include<utility>
#include<vector>
#include<string>
#include<iostream>

sdb::Interpreter::Interpreter() : disk_manager_(new sdb::DiskManager()){

}
sdb::Interpreter::~Interpreter() {
  delete disk_manager_;
}
void sdb::Interpreter::execute(sdb::Token& to_execute) {
  switch (to_execute.getTokenType()) {
  case Operation::READ:
    return;
  case Operation::WRITE:
    writeRowExecute(to_execute);
    return;
  case Operation::NEW:
    // same as open right now. disk manager opens/creates new file.
    disk_manager_->open(to_execute.get<MetaToken>()->getData());
    disk_manager_->zeroOutSlot(0);
    disk_manager_->closeCurrFile();
    disk_manager_->open(to_execute.get<MetaToken>()->getData());
    return;
  case Operation::OPEN:
    disk_manager_->open(to_execute.get<MetaToken>()->getData());
    return;
  case Operation::CLOSE:
    disk_manager_->closeCurrFile();
    return;
  case Operation::HELP:
    std::cout << "You have been helped." << std::endl;
    return;
  case Operation::CREATE:
    createTableExecute(to_execute);
    return;
  }
}

void sdb::Interpreter::createTableExecute(Token& to_execute) {
  if (!disk_manager_->isOpen()) return;
  sdb::SlottedPage* curr_db_page = disk_manager_->readFromSlot(0);
  for (int i = 0; i < curr_db_page->size(); ++i) {
    sdb::DBRow check_row{ curr_db_page->getBlock(i).first, curr_db_page->getBlock(i).second };
    if (check_row.getTableName() == to_execute.get<CreateTableToken>()->getTableName())
    {
      std::cout << "Table already exists!" << std::endl;
      return;
    }
  }
  auto allocated = curr_db_page->allocateBlock(sdb::DBRow::calcSizeRequired(to_execute.get<CreateTableToken>()->getTableName()));
  sdb::DBRow curr_db_row{ std::get<0>(allocated), std::get<1>(allocated) };
  curr_db_row.setTableName(to_execute.get<CreateTableToken>()->getTableName());
  std::array<std::byte, kPageSize>* test1 = new std::array<std::byte, kPageSize>{};
  sdb::SlottedPage* new_page = new SlottedPage(test1);
  size_t page_id_of_new_page = disk_manager_->append(new_page);
  curr_db_row.setPageId(page_id_of_new_page);
  sdb::SlottedPage* curr_tableheader_page = disk_manager_->readFromSlot(page_id_of_new_page);
  std::vector<std::string> column_names = to_execute.get<CreateTableToken>()->getColumnNames();
  std::vector<SQLType> column_types = to_execute.get<CreateTableToken>()->getColumnTypes();
  auto allocated_header = curr_tableheader_page->allocateBlock(sdb::TableHeaderRow::calcSizeRequired(column_names, column_types));
  sdb::TableHeaderRow curr_tableheader_row{ std::get<0>(allocated_header), std::get<1>(allocated_header) };
  curr_tableheader_row.loadData(column_names, column_types);
  disk_manager_->writeToSlot(curr_tableheader_page, page_id_of_new_page);
  disk_manager_->writeToSlot(curr_db_page, 0);
}

void sdb::Interpreter::writeRowExecute(Token& to_execute) {
  if (!disk_manager_->isOpen()) return;
  // find table header
  sdb::SlottedPage* curr_db_page = disk_manager_->readFromSlot(0);
  bool found_table{ false };
  sdb::DBRow check_row{nullptr, nullptr};
  for (int i = 0; i < curr_db_page->size(); ++i) {
    check_row = sdb::DBRow(curr_db_page->getBlock(i).first, curr_db_page->getBlock(i).second);
    if (check_row.getTableName() == to_execute.get<WriteToken>()->getTableName())
    {
      found_table = true;
      break;
    }
  }
  if (!found_table) {
    std::cout << "Table not found!" << std::endl;
    return;
  }
  sdb::SlottedPage* curr_table_header_page = disk_manager_->readFromSlot(check_row.getPageId());
  sdb::TableHeaderRow curr_header{ curr_table_header_page->getBlock(0).first, curr_table_header_page->getBlock(0).second };
  // prepare data to be loaded
  std::vector<int> int_load;
  std::vector<bool8_t> bool_load;
  std::vector<std::string> varchar_load;
  std::vector<std::string> datetime_load;
  if (curr_header.getNumOfCol() != to_execute.get<WriteToken>()->getData().size()) {
    std::cout << "Missing or excess columns!" << std::endl;
  }
  for (int i = 0; i < to_execute.get<WriteToken>()->getData().size(); ++i) {
    std::string to_convert = to_execute.get<WriteToken>()->getData()[i];
    switch (to_execute.get<WriteToken>()->getColumnTypes()[i]) {
    case sdb::SQLType::BOOLEAN:
      std::transform(to_convert.begin(), to_convert.end(), to_convert.begin(), [](unsigned char c) { return std::tolower(c); });
      bool_load.push_back(to_convert == std::string("true") ? true : false);
      break;
    case sdb::SQLType::VARCHAR:
      varchar_load.push_back(to_convert);
      break;
    case sdb::SQLType::INTEGER:
      int_load.push_back(stoi(to_convert));
      break;
    }
  }
  // find page with enough space
  uint16_t space_required = sdb::DataRow::calcSizeRequired(bool_load, int_load, datetime_load, varchar_load);
  std::pair<sdb::TablePointerRow, sdb::SlottedPage*> output_pair = findAvailableWrite(curr_table_header_page, space_required);
  sdb::TablePointerRow& ptr_to_insert_page = output_pair.first;
  sdb::SlottedPage* insert_page = output_pair.second;
  // write data
  if (!ptr_to_insert_page.empty()) {
    sdb::SlottedPage* curr_data_page = disk_manager_->readFromSlot(ptr_to_insert_page.getPageId());
    auto allocated = curr_data_page->allocateBlock(space_required);
    sdb::DataRow data_row{ std::get<0>(allocated), std::get<1>(allocated) };
    data_row.loadData(bool_load, int_load, datetime_load, varchar_load);
    disk_manager_->writeToSlot(curr_data_page, ptr_to_insert_page.getPageId());
    ptr_to_insert_page.setSpaceAvailable(ptr_to_insert_page.getSpaceAvailable() - space_required);
  }
  // write pointer to directory
}
std::pair<sdb::TablePointerRow, sdb::SlottedPage*> sdb::Interpreter::findAvailableWrite(SlottedPage* first_header_page, size_t wanted_size) {
  for (int i = 1; i < first_header_page->size(); ++i) {
    sdb::TablePointerRow space_available{ first_header_page->getBlock(i).first, first_header_page->getBlock(i).second };
    if (space_available.getSpaceAvailable() > wanted_size) {
      return  { space_available , first_header_page };
    }
  }
  while (first_header_page->getNextPage() != 0) {
    SlottedPage* curr_header_page = disk_manager_->readFromSlot(first_header_page->getNextPage());
    for (int i = 0; i < curr_header_page->size(); ++i) {
      sdb::TablePointerRow space_available{ curr_header_page->getBlock(i).first, curr_header_page->getBlock(i).second };
      if (space_available.getSpaceAvailable() > wanted_size) {
        return { space_available , first_header_page };
      }
    }
  }
  return { {nullptr, nullptr} , nullptr };
}
size_t sdb::Interpreter::appendNewPage() {
  std::array<std::byte, kPageSize>* test1 = new std::array<std::byte, kPageSize>{};
  sdb::SlottedPage* new_page = new SlottedPage(test1);
  size_t new_index = disk_manager_->append(new_page);
  return  new_index ;
}