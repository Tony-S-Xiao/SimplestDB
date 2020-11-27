#include"simplestdb_page_footer.h"
#include"simplestdb_page.h"
#include"simplestdb_disk_manager.h"
#include"simplestdb_row.h"

#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<fstream>

int main()
{
  sdb::DiskManager disk;
  disk.open("test_db.sdb");
  disk.zeroOutSlot(0);
  sdb::SlottedPage* test_page = disk.readFromSlot(0);
  std::tuple<void*, void*, unsigned short> block = test_page->allocateBlock(10);
  sdb::DBRow test_row{ std::get<0>(block), std::get<1>(block) };
  test_row.setSlotId(999);
  test_row.setTableName(std::string("wow i hope this works"));
  test_page->setNextPage(999);
  test_page->setPrevPage(99999);
  disk.writeToSlot(test_page, 0);
  return 0;
}