#include"simplestdb_tabledirectory.h"

sdb::TableDirectory::TableDirectory(sdb::DiskManager* disk_man, sdb::SlottedPage* input_page = nullptr) {
  header_page_ = input_page;
  curr_page_ = input_page;
  disk_man_ = disk_man;
}
sdb::TableDirectory::~TableDirectory() {

}
bool next();
TablePointerRow findAvailableSpace(size16_t);
void associateHeaderPage(sdb::SlottedPage*);