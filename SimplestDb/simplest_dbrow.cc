#include"simplestdb_dbrow.h"
#include"simplestdb_row.h"

#include<string>
#include<cstddef>
sdb::DBRow::DBRow(std::byte* begin, std::byte* end) : Row(begin, end) {
  page_id_ = reinterpret_cast<OnDiskPointer*>(end) - 1;
  tablename_end_ = reinterpret_cast<std::byte*>(page_id_);
}
std::string sdb::DBRow::getTableName() {
  return std::string(reinterpret_cast<char*>(row_begin_), tablename_end_ - row_begin_);
}
sdb::OnDiskPointer sdb::DBRow::getPageId() {
  return *page_id_;
}
void sdb::DBRow::setTableName(const std::string& name) {
  memcpy(reinterpret_cast<char*>(row_begin_), name.data(), std::min(name.size(), row_end_ - row_begin_ - sizeof(uint32_t)));
}
void sdb::DBRow::setPageId(uint32_t page_id) {
  *page_id_ = page_id;
}
size_t sdb::DBRow::calcSizeRequired(const std::string& name) {
  return name.size() + sizeof(unsigned int);
}