#include"simplestdb_tabledirectory.h"
#include"simplestdb_tablepointerrow.h"
#include"simplestdb_page.h"
#include"simplestdb_disk_manager.h"

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

}
void sdb::TableDirectory::associateHeaderPage(sdb::SlottedPage* header_page) {
  header_page_ = header_page;
}
sdb::SlottedPage* sdb::TableDirectory::getCurrPage() {
  return curr_page_;
}
bool sdb::TableDirectory::nextPage() {

}
void sdb::TableDirectory::reset() {
  curr_page_ = header_page_;
}