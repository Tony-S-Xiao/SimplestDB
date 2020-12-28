#ifndef SIMPLESTDB_PARSER_H_
#define SIMPLESTDB_PARSER_H_
#include"simplestdb_token.h"
#include"simplestdb_enum_operation.h"
#include"simplestdb_parser_statemachine.h"

#include<string>
#include<vector>
#include<unordered_map>
#include<unordered_set>

namespace sdb {
// Allocates a new token. 
// Delete after use.

class Parser {
 public:
  Parser();
  std::unique_ptr<Token> parse(std::string);
 private:
  const std::unordered_map<std::string, SMType> lookup_ = {
    {".open", SMType::METAOPENTOKEN},
    {".close", SMType::METACLOSETOKEN},
    {".help", SMType::METAHELPTOKEN},
    {".new", SMType::METANEWTOKEN},
    {"create", SMType::SQLCREATE},
    {"select", SMType::SQLSELECT},
    {"from", SMType::SQLFROM},
    {"where", SMType::SQLWHERE},
    {"insert", SMType::SQLINSERT},
    {"into", SMType::SQLINTO},
    {"table", SMType::SQLTABLE},
    {"int", SMType::SQLTYPEINT},
    {"varchar", SMType::SQLTYPEVARCHAR},
    {"boolean", SMType::SQLTYPEBOOL},
    {"values", SMType::SQLTYPEVALUE},
    {"*", SMType::SQLALL}
  };
  std::vector<SMToken> tokenize(std::string);
  StateMachine sm_{};
};
}//namespace sdb

#endif // !SIMPLESTDB_PARSER_H_