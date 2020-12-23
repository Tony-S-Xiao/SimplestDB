#ifndef SIMPLESTDB_TOKEN_H_
#define SIMPLESTDB_TOKEN_H_
#include"simplestdb_enum_operation.h"
#include"simplestdb_token_meta.h"
#include"simplestdb_token_sqlcreate.h"
#include"simplestdb_token_sqlquery.h"
#include"simplestdb_token_sqlwritetoken.h"

#include<string>
#include<vector>
#include<memory>

namespace sdb {
// This class is the output of the parser.
// Aggragates some different token types.
// Can be either meta token or sql token.
class Token {
 public:
  Token(std::unique_ptr<MetaToken>&&, Operation operation = Operation::NUL);
  Token(std::unique_ptr<CreateTableToken>&&, Operation operation = Operation::NUL);
  Token(std::unique_ptr<QueryToken>&&, Operation operation = Operation::NUL);
  Token(std::unique_ptr<WriteToken>&&, Operation operation = Operation::NUL);
  Token(MetaToken*, Operation operation = Operation::NUL);
  Token(CreateTableToken*, Operation operation = Operation::NUL);
  Token(QueryToken*, Operation operation = Operation::NUL);
  Token(WriteToken*, Operation operation = Operation::NUL);
  Token();
  // Need to call the right template for the correct type of token
  // Returns nullptr if not correct.
  template<class C>
  C* get();
  void setWellFormedFlag(bool);
  bool getWellFormedFlag();
  void setOperationType(Operation);
  // This is used to determine what type of token this object is.
  // See Operation.
  Operation getTokenType();
 private:
  // Tokens should be well formed before any operation is done with them.
  bool well_formed_{ false };
  Operation operation_{ Operation::NUL };
  // Only one of these is non nullptr. 
  std::unique_ptr<MetaToken> meta_token_ptr_{};
  std::unique_ptr<CreateTableToken> create_table_token_ptr_{};
  std::unique_ptr<QueryToken> query_token_ptr_{};
  std::unique_ptr<WriteToken> write_token_ptr_{};
};
}//namespace sdb
#endif // !SIMPLESTDB_TOKEN_H_