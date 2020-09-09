#pragma once
#include<string>

enum class Command { INSERT, EXIT, NONE, QUERY, CONFIG };

namespace sdb {

class Token {
 public:
  Token();
  Token(Command, std::string);
  ~Token();
  std::string getText();

 private:
  Command type;
  std::string data;
};

}//namespace sdb

