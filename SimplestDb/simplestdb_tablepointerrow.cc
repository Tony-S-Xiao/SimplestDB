#include"simplestdb_tablepointerrow.h"
#include"simplestdb_row.h"

#include<cstddef>
sdb::TablePointerRow::TablePointerRow(std::byte* begin, std::byte* end) : Row(begin, end) {
  page_id_ = reinterpret_cast<OnDiskPointer*>(begin);
  space_available_on_page_ = reinterpret_cast<uint16_t*>(page_id_ + 1);
}
void sdb::TablePointerRow::setPageId(OnDiskPointer index) {
  *page_id_ = index;
}
void sdb::TablePointerRow::setSpaceAvailable(uint16_t space) {
  *space_available_on_page_ = space;
}
unsigned int sdb::TablePointerRow::getPageId() {
  return *page_id_;
}
unsigned short sdb::TablePointerRow::getSpaceAvailable() {
  return *space_available_on_page_;
}