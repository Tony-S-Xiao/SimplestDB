#ifndef SIMPLESTDB_INTERPRETER_H_
#define SIMPLESTDB_INTERPRETER_H_
#include"simplestdb_token.h"
#include"simplestdb_disk_manager.h"

namespace sdb {

class Interpreter {
 public:
  Interpreter();
  ~Interpreter();
  void execute(Token&);
 private:
  sdb::DiskManager* disk_manager_{};
};
}

#endif // !SIMPLESTDB_INTERPRETER_H_
