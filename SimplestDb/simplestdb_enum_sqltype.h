#ifndef SIMPLESTDB_ENUM_SQLTYPE_H_
#define SIMPLESTDB_ENUM_SQLTYPE_H_
namespace sdb {
// Denotes the data type of the value.
enum class SQLType : unsigned char {
  NUL = 0,
  VARCHAR = 1 << 0,
  BOOLEAN = 1 << 1,
  INTEGER = 1 << 2,
  DATETIME = 1 << 3
};
}  // namespace sdb
#endif // !SIMPLESTDB_ENUM_SQLTYPE_H_

