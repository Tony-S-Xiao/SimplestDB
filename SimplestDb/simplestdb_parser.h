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

 private:
  Parser();
  enum class SMType {
    NUL,
    METAOPENTOKEN,
    METACLOSETOKEN,
    METAHELPTOKEN,
    METANEWTOKEN,
    SQLSELECT,
    SQLCREATE,
    SQLTABLE,
    SQLFROM,
    SQLWHERE,
    SQLINSERT,
    SQLTABLE,
    SQLTYPEINT,
    SQLTYPEVARCHAR,
    SQLTYPEBOOL
  };
  struct SMToken {
    SMType type_{ SMType::NUL };
    std::string data_{ "" };
  };
  static const inline std::unordered_map<std::string, SMType> lookup_ = {
    {".open", SMType::METAOPENTOKEN},
    {".close", SMType::METACLOSETOKEN},
    {".help", SMType::METAHELPTOKEN},
    {".new", SMType::METANEWTOKEN},
    {"create", SMType::SQLCREATE},
    {"select", SMType::SQLSELECT},
    {"from", SMType::SQLFROM},
    {"where", SMType::SQLWHERE},
    {"insert", SMType::SQLINSERT},
    {"table", SMType::SQLTABLE},
    {"int", SMType::SQLTYPEINT},
    {"varchar", SMType::SQLTYPEVARCHAR},
    {"boolean", SMType::SQLTYPEBOOL}
  };
  static std::vector<SMToken> tokenize(std::string);
};
}//namespace sdb

#endif // !SIMPLESTDB_PARSER_H_