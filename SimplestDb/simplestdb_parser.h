#ifndef SIMPLESTDB_PARSER_H_
#define SIMPLESTDB_PARSER_H_
#include"simplestdb_token.h"
#include"simplestdb_enum_operation.h"

#include<string>
#include<vector>
#include<unordered_map>
#include<unordered_set>

namespace sdb {

class Parser {
 public:
  //deletes input vector
  Token createToken(const std::string& input);
 private:
  const std::unordered_set<std::string>
    sql_command_set{ "create", "table", "select", "from", "where", "into", "values", "insert" };
  const std::unordered_map<std::string, sdb::Operation>
    meta_command_set{ { "help", sdb::Operation::HELP }, {"close", sdb::Operation::CLOSE },
      { "open", sdb::Operation::OPEN }, { "new", sdb::Operation::NEW } };
  Token createSQLToken(const std::string&);
  Token createMetaToken(const std::string&);
};

}//namespace sdb

#endif // !SIMPLESTDB_PARSER_H_

