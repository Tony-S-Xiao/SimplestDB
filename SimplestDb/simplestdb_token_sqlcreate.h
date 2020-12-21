#ifndef SIMPLESTDB_TOKEN_SQLCREATE_H_
#define SIMPLESTDB_TOKEN_SQLCREATE_H_

#include"simplestdb_token_sql.h"

namespace sdb {

class CreateTableToken : public SQLToken {
 public:
   CreateTableToken();
 private:
};
}  // namespace sdb


#endif // !SIMPLESTDB_TOKEN_SQLCREATE_H_
