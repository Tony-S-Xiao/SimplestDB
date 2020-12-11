#ifndef SIMPLESTDB_TABLEPOINTERROW_H_
#define SIMPLESTDB_TABLEPOINTERROW_H_

#include"simplestdb_row.h"

#include<cstddef>
#include<cstdint>

namespace sdb {

// Contains on-disk pointer(page id), space available pairs.
// Used to help with faster insertions and finding all data that
// belong to a given table.
class TablePointerRow : public Row {
public:
  TablePointerRow(std::byte*, std::byte*);
  void setPageId(OnDiskPointer index);
  void setSpaceAvailable(uint16_t space);
  unsigned int getPageId();
  unsigned short getSpaceAvailable();
private:
  uint16_t* space_available_on_page_;
  OnDiskPointer* page_id_;
};

}  // namspace sdb
#endif // !SIMPLESTDB_TABLEPOINTERROW_H_
