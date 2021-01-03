#include"simplestdb_tabledirectory.h"
#include"simplestdb_tablepointerrow.h"
#include"simplestdb_page.h"
#include"simplestdb_disk_manager.h"

#include<utility>
sdb::TableDirectory::TableDirectory(sdb::DiskManager* disk_man, sdb::SlottedPage* input_page = nullptr) {
  header_page_ = input_page;
  curr_page_ = input_page;
  disk_man_ = disk_man;
}
sdb::TableDirectory::~TableDirectory() {
  disk_man_->writeToSlot(curr_page_, curr_page_->getPageId());
  disk_man_->writeToSlot(header_page_, header_page_->getPageId());
}
sdb::TablePointerRow sdb::TableDirectory::findAvailableSpace(size16_t size_required) {
  // first block of first page is the table header
  int i = curr_page_ == header_page_ ? 1 : 0;
  for (; i < curr_page_->size(); ++i) {
    auto block = curr_page_->getBlock(i);
    TablePointerRow curr_ptr_row{ block.first, block.second };
    if (curr_ptr_row.getSpaceAvailable() >= size_required) {
      return curr_ptr_row;
    }
  }
  while (nextPage()) {
    for (int i = 0; i < curr_page_->size(); ++i) {
      auto block = curr_page_->getBlock(i);
      TablePointerRow curr_ptr_row{ block.first, block.second };
      if (curr_ptr_row.getSpaceAvailable() >= size_required) {
        return curr_ptr_row;
      }
    }
  }
  return { nullptr, nullptr };
}
void sdb::TableDirectory::associateHeaderPage(sdb::SlottedPage* header_page) {
  header_page_ = header_page;
}
sdb::SlottedPage* sdb::TableDirectory::getCurrPage() {
  return curr_page_;
}
bool sdb::TableDirectory::nextPage() {
  if (curr_page_->getNextPage() != 0) {
    curr_page_ = disk_man_->readFromSlot(curr_page_->getNextPage());
    return true;
  }
  return false;
}
bool sdb::TableDirectory::prevPage() {
  if (curr_page_->getPrevPage() != 0) {
    curr_page_ = disk_man_->readFromSlot(curr_page_->getPrevPage());
    return true;
  }
  return false;
}
void sdb::TableDirectory::reset() {
  curr_page_ = header_page_;
}