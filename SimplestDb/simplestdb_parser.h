#ifndef SIMPLESTDB_PARSER_H_
#define SIMPLESTDB_PARSER_H_
#include"simplestdb_token.h"
#include"simplestdb_enum_operation.h"

#include<string>
#include<vector>
#include<unordered_map>
#include<unordered_set>

namespace sdb {
// Allocates a new token. 
// Delete after use.
class Parser {
 public:
  Token* createToken(const std::string& input);
  Token* createToken(const char* input);
 private:
  const std::unordered_map<std::string, sdb::Operation>
    sql_command_set{ { "create", sdb::Operation::CREATE }, {"table", sdb::Operation::CREATE},
      {"select", sdb::Operation::READ}, {"from", sdb::Operation::NUL}, {"where",sdb::Operation::NUL},
      {"into", sdb::Operation::NUL}, {"values", sdb::Operation::NUL}, {"insert", sdb::Operation::WRITE} };
  const std::unordered_map<std::string, sdb::Operation>
    meta_command_set{ { "help", sdb::Operation::HELP }, {"close", sdb::Operation::CLOSE },
      { "open", sdb::Operation::OPEN }, { "new", sdb::Operation::NEW } };
  Token* createSQLToken(const std::string&);
  Token* createMetaToken(const std::string&);
};
}//namespace sdb

#endif // !SIMPLESTDB_PARSER_H_