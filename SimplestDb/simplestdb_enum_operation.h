#ifndef SIMPLESTDB_ENUM_OPERATION_H_
#define SIMPLESTDB_ENUM_OPERATION_H_
namespace sdb {
// Denotes the operation being done.
enum class Operation : unsigned char { 
  NUL = 0,
  READ = 1 << 0,
  WRITE = 1 << 1,
  NEW = 1 << 2,
  OPEN = 1 << 3,
  CLOSE = 1 << 4,
  HELP = 1 << 5,
  CREATE = 1 << 6
};
}  // namespace sdb
#endif // !SIMPLESTDB_ENUM_OPERATION_H_
