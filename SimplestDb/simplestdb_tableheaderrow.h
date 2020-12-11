#ifndef SIMPLESTDB_TABLEHEADERROW_H_
#define SIMPLESTDB_TABLEHEADERROW_H_

#include"simplestdb_row.h"
#include"simplestdb_token.h"

#include<cstddef>
#include<cstdint>
#include<vector>
namespace sdb {
// Contains the header/column name and type information for the table. 
// Acts as an array of column name, column type pairs.
class TableHeaderRow : public Row {
 public:
  TableHeaderRow(std::byte*, std::byte*);
  unsigned short getNumOfCol();
  std::string getColumnName(int i);
  SQLType getColumnType(int i);
  void loadData(const std::vector<std::string>& all_column_names,
    const std::vector<SQLType>& all_types);
  static size_t calcSizeRequired(const std::vector<std::string>& all_column_names,
    const std::vector<SQLType>& all_types);
private:
  uint16_t* num_of_col_;
  uint16_t* start_of_pointers_;
};
}  // namspace sdb
#endif // !SIMPLESTDB_TABLEHEADERROW_H_
