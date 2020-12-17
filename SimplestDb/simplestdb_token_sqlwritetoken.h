#ifndef SIMPLESTDB_TOKEN_SQLWRITETOKEN_H_
#define SIMPLESTDB_TOKEN_SQLWRITETOKEN_H_
#include"simplestdb_token.h"
#include"simplestdb_token_sql.h"

#include<string>
#include<vector>
namespace sdb {
class WriteToken : public SQLToken {
public:
  void appendData(std::string);
  std::vector<std::string> getData();
private:
  std::vector<std::string> data_;
};
}
#endif // !1



