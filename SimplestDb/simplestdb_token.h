#ifndef SIMPLESTDB_TOKEN_H_
#define SIMPLESTDB_TOKEN_H_

#include<string>
#include<vector>

namespace sdb {
// Denotes the data type of the value.
enum class SQLType : unsigned char { 
  NUL = 0,
  VARCHAR = 1 << 0,
  BOOLEAN = 1 << 1,
  INTEGER = 1 << 2,
  DATETIME = 1 << 3
};
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

class Token {
 public:
  void setWellFormedFlag(bool);
  bool getWellFormedFlag();
  void setTokenType(Operation);
  Operation getTokenType();
 protected:
  Token();
 private:
  // Tokens should be well formed before any operation is done with them.
  bool well_formed_{ false };
  Operation operation_{ Operation::NUL };
};

}//namespace sdb

#endif // !SIMPLESTDB_TOKEN_H_