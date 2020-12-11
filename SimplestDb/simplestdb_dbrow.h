#ifndef SIMPLEST_DBROW_H_
#define SIMPLEST_DBROW_H_

#include"simplestdb_row.h"

#include<string>
#include<cstdint>
namespace sdb {
// Contains the table name and an on-disk pointer(page id) to 
// the table.
class DBRow : public Row {
public:
  DBRow(std::byte*, std::byte*);
  std::string getTableName();
  OnDiskPointer getPageId();
  void setTableName(const std::string& name);
  void setPageId(uint32_t page_id);
  static size_t calcSizeRequired(const std::string& name);
private:
  OnDiskPointer* page_id_;
  std::byte* tablename_end_;
};

} //  namespace sdb
#endif // !SIMPLEST_DBROW_H_