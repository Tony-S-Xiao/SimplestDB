#ifndef SIMPLESTDB_PARSER_STATEMACHINE_H_
#define SIMPLESTDB_PARSER_STATEMACHINE_H_

#include"simplestdb_token.h"
#include<vector>
#include<memory>

namespace sdb {

enum class SMType {
  NUL,
  METAOPENTOKEN,
  METACLOSETOKEN,
  METAHELPTOKEN,
  METANEWTOKEN,
  SQLALL,
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
class StateMachine {
public:
  std::unique_ptr<Token> createToken(std::vector<SMToken>&&);
  void reset();
private:
  bool next(SMToken);
  void clear();
  int state_{ 0 };
  std::unique_ptr<Token> token_ptr_{ nullptr };
};
}  // namespace sdb
#endif // !SIMPLESTDB_PARSER_STATEMACHINE_H_