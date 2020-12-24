#include"simplestdb_parser.h"
#include"simplestdb_token.h"

#include<string>
#include<vector>
#include<memory>
#include<cstdint>
#include<algorithm>

std::vector<sdb::Parser::SMToken> sdb::Parser::tokenize(std::string input) {
  std::string copy(input);
  std::vector<sdb::Parser::SMToken> res{};
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
    int next_white_space{ copy.find_first_of(i, ' ') };
    to_be_tokenized.push_back(copy.substr(i, next_white_space));
    original_str.push_back(input.substr(i, next_white_space));
    while (copy[next_white_space] == ' ') {
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