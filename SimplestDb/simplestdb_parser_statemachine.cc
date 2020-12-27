#include"simplestdb_parser_statemachine.h"
#include"simplestdb_token.h"

std::unique_ptr<sdb::Token> sdb::StateMachine::createToken(std::vector<SMToken>&& token_list) {
  for (SMToken& token : token_list) {
    if (!next(token))
      return { nullptr };
  }
  return std::move(token_ptr_);
}
void sdb::StateMachine::clear() {
  token_ptr_ = nullptr;
}

bool sdb::StateMachine::next(SMToken next_state) {
  switch (state_) {
  case 0:
    switch (next_state.type_) {
    case SMType::METAOPENTOKEN:
      token_ptr_ = std::unique_ptr<Token>(new Token(new MetaToken(), Operation::OPEN));
      state_ = 1;
      return true;
    case SMType::METACLOSETOKEN:
      token_ptr_ = std::unique_ptr<Token>(new Token(new MetaToken(), Operation::CLOSE));
      state_ = 1;
      return true;
    case SMType::METAHELPTOKEN:
      token_ptr_ = std::unique_ptr<Token>(new Token(new MetaToken(), Operation::HELP));
      state_ = 1;
      return true;
    case SMType::METANEWTOKEN:
      token_ptr_ = std::unique_ptr<Token>(new Token(new MetaToken(), Operation::NEW));
      state_ = 1;
      return true;
    case SMType::SQLSELECT:
      token_ptr_ = std::unique_ptr<Token>(new Token(new QueryToken(), Operation::READ));
      state_ = 4;
      return true;
    case SMType::SQLCREATE:
      token_ptr_ = std::unique_ptr<Token>(new Token(new CreateTableToken(), Operation::CREATE));
      state_ = 10;
      return true;
    case SMType::SQLINSERT:
      token_ptr_ = std::unique_ptr<Token>(new Token(new WriteToken(), Operation::WRITE));
      state_ = 14;
      return true;
    }
    break;
  case 1:
    token_ptr_->get<MetaToken>()->setData(next_state.data_);
    state_ = 0;
    return true;
  case 2:

    break;
  case 3:

    break;
  case 4:

    break;
  case 5:

    break;
  case 6:

    break;
  case 7:

    break;
  case 8:

    break;
  case 9:

    break;
  case 10:

    break;
  case 11:

    break;
  case 12:

    break;
  case 13:

    break;
  case 14:

    break;
  case 15:

    break;
  case 16:

    break;
  case 17:

    break;
  case 18:

    break;
  default:
    reset();
  }
  return false;
}

void sdb::StateMachine::reset() {
  state_ = 1;
  clear();
}