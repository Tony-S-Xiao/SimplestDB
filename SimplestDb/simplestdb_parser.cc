#include"simplestdb_parser.h"
#include"simplestdb_token.h"
#include"simplestdb_parser_statemachine.h"

#include<string>
#include<vector>
#include<memory>
#include<cstdint>
#include<algorithm>

sdb::Parser::Parser() : sm_(){
}
std::unique_ptr<sdb::Token> sdb::Parser::parse(std::string input) {
  std::unique_ptr<sdb::Token> curr = sm_.createToken(tokenize(input));
  sm_.reset();
  if (curr != nullptr && 
    curr->getTokenType() == Operation::WRITE &&
    curr->get<WriteToken>()->getColumnNames().size() != curr->get<WriteToken>()->getData().size()) {
      return nullptr;
  }
  return curr;
}
std::vector<sdb::SMToken> sdb::Parser::tokenize(std::string input) {
  std::string copy(input);
  std::vector<sdb::SMToken> res{};
  std::for_each(copy.begin(), copy.end(), [](char& c) {
    if (c >= 'A' && c <= 'Z') c = tolower(c);
    });
  for (int i = 0; i < copy.size(); ++i) {
    if (copy[i] == '(' ||
      copy[i] == ',' ||
      copy[i] == ')')
      copy[i] = ' ';
  }
  std::vector<std::string> to_be_tokenized{};
  std::vector<std::string> original_str{};
  for (int i = 0; i < copy.size(); ++i) {
    size_t next_white_space{ copy.find_first_of( ' ', i) };
    to_be_tokenized.push_back(copy.substr(i, next_white_space - i));
    original_str.push_back(input.substr(i, next_white_space - i));
    while (next_white_space < copy.size() && copy[next_white_space] == ' ') {
      ++next_white_space;
    }
    i = next_white_space - 1;
  }
  for (int i = 0; i < to_be_tokenized.size(); ++i) {
    if (lookup_.find(to_be_tokenized[i]) != lookup_.end())
      res.push_back(SMToken{ lookup_.find(to_be_tokenized[i])->second, original_str[i] });
    else 
      res.push_back(SMToken{ SMType::NUL, original_str[i] });
  }
  return res;
} 
