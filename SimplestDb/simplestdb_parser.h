#pragma once
#include<string>
#include<unordered_map>


namespace sdb {

class Parser {
 public:
  ~Parser();
  Parser();
  Token* createToken(std::string);
 private:
};

}//namespace sdb


