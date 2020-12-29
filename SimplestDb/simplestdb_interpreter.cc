#include"simplestdb_interpreter.h"
#include"simplestdb_token.h"
#include"simplestdb_enum_operation.h"
#include"simplestdb_disk_manager.h"
#include"simplestdb_dbrow.h"
#include"simplestdb_tableheaderrow.h"

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
    return;
  case Operation::NEW:
    // same as open right now. disk manager opens/creates new file.
    disk_manager_->open(to_execute.get<MetaToken>()->getData());
    disk_manager_->zeroOutSlot(0);
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
    if (!disk_manager_->isOpen()) return;
    sdb::SlottedPage* curr_db_page = disk_manager_->readFromSlot(0);
    auto allocated = curr_db_page->allocateBlock(sdb::DBRow::calcSizeRequired(to_execute.get<CreateTableToken>()->getTableName()));
    sdb::DBRow curr_db_row{ std::get<0>(allocated), std::get<1>(allocated) };
    curr_db_row.setTableName(to_execute.get<CreateTableToken>()->getTableName());
    sdb::SlottedPage* new_page = new SlottedPage({});
    size_t page_id_of_new_page = disk_manager_->append(new_page);
    delete new_page;
    curr_db_row.setPageId(page_id_of_new_page);
    sdb::SlottedPage* curr_tableheader_page = disk_manager_->readFromSlot(page_id_of_new_page);
    std::vector<std::string> column_names = to_execute.get<CreateTableToken>()->getColumnNames();
    std::vector<SQLType> column_types = to_execute.get<CreateTableToken>()->getColumnTypes();
    auto allocated_header = curr_tableheader_page->allocateBlock(sdb::TableHeaderRow::calcSizeRequired(column_names, column_types));
    sdb::TableHeaderRow curr_tableheader_row{ std::get<0>(allocated_header), std::get<1>(allocated_header) };
    curr_tableheader_row.loadData(column_names, column_types);
    disk_manager_->writeToSlot(curr_tableheader_page, page_id_of_new_page);
    return;
  }
}