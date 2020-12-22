#ifndef SIMPLESTDB_TOKEN_SQLQUERY_H_
#define SIMPLESTDB_TOKEN_SQLQUERY_H_
#include"simplestdb_token_sql.h"
#include<string>
namespace sdb {

class QueryToken : public SQLToken {
 public:

  void setCondition(std::string);
  std::string getCondition();
 private:
  std::string condition_;
};
}  // namespace sdb
#endif // !SIMPLESTDB_TOKEN_SQLQUERY_H_