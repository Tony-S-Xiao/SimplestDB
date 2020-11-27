#pragma once
#include"simplestdb_token.h"
#include<string>
#include<vector>
#include<unordered_set>


namespace sdb {

const std::unordered_set<std::string> SQLCommandSet{ "create", "table", "select", "from", "where", "into", "values", "insert" };

class Parser {
 public:
  ~Parser();
  Parser();

  //deletes input vector
  Token* createToken(std::vector<std::string>*);

  //creates new vector. delete after use
  std::vector<std::string>* getCommand();
 private:
};

}//namespace sdb