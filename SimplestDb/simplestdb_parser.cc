#include"simplestdb_parser.h"
#include"simplestdb_token.h"

#include<string>
#include<vector>
#include<memory>
#include<cstdint>

sdb::Token sdb::Parser::createToken(const std::string& input) {
  return input.size() == 0 ? Token() :
    input[0] == '.' ? createMetaToken(input) :
    createSQLToken(input);
}
sdb::Token sdb::Parser::createSQLToken(const std::string& input) {

}
sdb::Token sdb::Parser::createMetaToken(const std::string& input) {
  std::vector<std::string> sep_by_delimiter{};
  for (int i = 1; i < input.size(); ++i) {
    size_t next_white_space = input.find_first_of(' ', i);
    if (next_white_space != std::string::npos) {
      sep_by_delimiter.push_back(input.substr(i, next_white_space - i));
      i = next_white_space;
      while (input[i] == ' ') ++i;
    }
  }
  if (meta_command_set.find(sep_by_delimiter[0]) == meta_command_set.end())
    return sdb::Token();

  return sdb::Token();
}