#ifndef SIMPLESTDB_TOKEN_META_H_
#define SIMPLESTDB_TOKEN_META_H_
#include"simplestdb_token.h"

#include<string>

namespace sdb {

class MetaToken : public Token {
 public:
  void setData(std::string);
  std::string getData();
 private:
  std::string data_;
};
}  // namespace sdb
#endif // !SIMPLESTDB_TOKEN_META_H_


