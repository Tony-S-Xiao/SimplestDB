#include"simplestdb_parser.h"
#include"simplestdb_token.h"

#include<string>
#include<vector>
#include<memory>
#include<cstdint>

sdb::Token* sdb::Parser::createToken(const std::string& input) {
  return input.size() == 0 ? new Token() :
    input[0] == '.' ? createMetaToken(input) :
    createSQLToken(input);
}
sdb::Token* sdb::Parser::createToken(const char* input) {
  std::string input_1(input);
  return createToken(input_1);
}
sdb::Token* sdb::Parser::createSQLToken(const std::string& input) {
  return new Token();
}
sdb::Token* sdb::Parser::createMetaToken(const std::string& input) {
  std::vector<std::string> sep_by_delimiter{};
  for (int i = 1; i < input.size();) {
    size_t next_white_space = input.find_first_of(' ', i);
    if (next_white_space != std::string::npos) {
      sep_by_delimiter.push_back(input.substr(i, next_white_space - i));
      i = next_white_space;
      while (input[i] == ' ') ++i; // This is what increments i.
    }
    else {
      sep_by_delimiter.push_back(input.substr(i));
      break;
    }
  }
  if (meta_command_set.find(sep_by_delimiter[0]) == meta_command_set.end())
    return new sdb::Token();
  sdb:Token* result_token{ nullptr };
  switch (meta_command_set.find(sep_by_delimiter[0])->second) {
  case sdb::Operation::HELP:
    result_token = new Token(new MetaToken(), sdb::Operation::HELP);
    return result_token;
  case sdb::Operation::CLOSE:
    result_token = new Token(new MetaToken(), sdb::Operation::CLOSE);
    return result_token;
  case sdb::Operation::OPEN:
    result_token = new Token(new MetaToken(), sdb::Operation::OPEN);
    break;
  case sdb::Operation::NEW:
    result_token = new Token(new MetaToken(), sdb::Operation::NEW);
    break;
  }
  result_token->get<MetaToken>()->setData(sep_by_delimiter[1]);
  return result_token;
}