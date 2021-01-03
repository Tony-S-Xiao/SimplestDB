#ifndef SIMPLESTDB_TABLEDIRECTORY_H_
#define SIMPLESTDB_TABLEDIRECTORY_H_

#include"simplestdb_page.h"
#include"simplestdb_tablepointerrow.h"

#include<string>
#include<vector>

namespace sdb {

class DiskManager;

class TableDirectory {
public:
  TableDirectory(sdb::DiskManager*, sdb::SlottedPage*);
  ~TableDirectory();
  TablePointerRow findAvailableSpace(size16_t);
  void associateHeaderPage(sdb::SlottedPage*);
  SlottedPage* getCurrPage();
  bool nextPage();
  bool prevPage();
  void reset();
private:
  DiskManager* disk_man_{ nullptr };
  SlottedPage* curr_page_{ nullptr };
  SlottedPage* header_page_{ nullptr };
};
}  // namespace sdb
#endif // !SIMPLESTDB_TABLEDIRECTORY_H_
