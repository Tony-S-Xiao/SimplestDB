#ifndef SIMPLESTDB_INTERPRETER_H_
#define SIMPLESTDB_INTERPRETER_H_
#include"simplestdb_token.h"
#include"simplestdb_disk_manager.h"

#include<utility>

namespace sdb {

class Interpreter {
 public:
  Interpreter();
  ~Interpreter();
  void execute(Token&);
 private:
  sdb::DiskManager* disk_manager_{};
  void createTableExecute(Token&);
  void writeRowExecute(Token&);
  //returns the index of the data page 
  size_t findAvailableWrite(SlottedPage* first_header_page, size_t wanted_size);
  size_t appendNewPage();
};
}
#endif // !SIMPLESTDB_INTERPRETER_H_
