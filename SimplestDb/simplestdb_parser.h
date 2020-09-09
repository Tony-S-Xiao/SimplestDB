#pragma once
#include<string>
#include<unordered_map>

#include"simplestdb_token.h"

namespace sdb {

class Parser {
 public:
  ~Parser();
  Parser();
  Token* initToken(std::string);

 private:
  Token* token;
};

}//namespace sdb


