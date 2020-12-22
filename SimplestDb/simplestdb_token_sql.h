#ifndef SIMPLESTDB_TOKEN_SQL_H_
#define SIMPLESTDB_TOKEN_SQL_H_

#include"simplestdb_enum_sqltype.h"

#include<string>
#include<vector>

namespace sdb {
// This class is the base class for the sql operation tokens.
class SQLToken {
public:
  void setTableName(std::string);
  std::string getTableName();
  void pushBackColumnName(std::string);
  void pushBackColumnType(SQLType);
  std::vector<std::string> getColumnNames();
  std::vector<SQLType> getColumnTypes();
protected:
  SQLToken();
private:
  std::string table_name_{};
  std::vector<std::string> column_names_{};
  std::vector<SQLType> column_types_{};
};
}  // namespace sdb
#endif // !SIMPLESTDB_TOKEN_SQL_H_
