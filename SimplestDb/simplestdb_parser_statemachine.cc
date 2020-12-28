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
    if (next_state.type_ == SMType::NUL) {
      token_ptr_->get<QueryToken>()->pushBackColumnName(next_state.data_);
      state_ = 5;
      return true;
    }
    else if (next_state.type_ == SMType::SQLALL) {
      token_ptr_->get<QueryToken>()->pushBackColumnName("*");
      state_ = 5;
      return true;
    }
    break;
  case 5:
    if (next_state.type_ == SMType::SQLFROM) {
      state_ = 7;
      return true;
    }
    break;
  case 6:

    break;
  case 7:
    if (next_state.type_ == SMType::NUL) {
      token_ptr_->get<QueryToken>()->setTableName(next_state.data_);
      state_ = 8;
      return true;
    }
    break;
  case 8:
    if (next_state.type_ == SMType::SQLWHERE) {
      token_ptr_->get<QueryToken>()->setCondition(next_state.data_);
      state_ = 9;
      return true;
    }
    break;
  case 9:

    break;
  case 10:
    if (next_state.type_ == SMType::SQLTABLE) {
      state_ = 11;
      return true;
    }
    break;
  case 11:
    if (next_state.type_ == SMType::NUL) {
      token_ptr_->get<CreateTableToken>()->setTableName(next_state.data_);
      state_ = 12;
      return true;
    }
    break;
  case 12:
    if (next_state.type_ == SMType::NUL) {
      token_ptr_->get<CreateTableToken>()->pushBackColumnName(next_state.data_);
      state_ = 13;
      return true;
    }
    break;
  case 13:
    if (next_state.type_ == SMType::SQLTYPEBOOL) {
      token_ptr_->get<CreateTableToken>()->pushBackColumnType(SQLType::BOOLEAN);
      state_ = 12;
      return true;
    } else if (next_state.type_ == SMType::SQLTYPEINT) {
      token_ptr_->get<CreateTableToken>()->pushBackColumnType(SQLType::INTEGER);
      state_ = 12;
      return true;
    } else if (next_state.type_ == SMType::SQLTYPEVARCHAR) {
      token_ptr_->get<CreateTableToken>()->pushBackColumnType(SQLType::VARCHAR);
      state_ = 12;
      return true;
    }
    break;
  case 14:
    if (next_state.type_ == SMType::SQLINTO) {
      state_ = 15;
      return true;
    }
    break;
  case 15:
    if (next_state.type_ == SMType::NUL) {
      token_ptr_->get<WriteToken>()->setTableName(next_state.data_);
      state_ = 16;
      return true;
    }
    break;
  case 16:
    if (next_state.type_ == SMType::NUL) {
      token_ptr_->get<WriteToken>()->pushBackColumnName(next_state.data_);
      state_ = 16;
      return true;
    }
    else if (next_state.type_ == SMType::SQLTYPEVALUE) {
      state_ = 18;
      return true;
    }
    break;
  case 17:

    break;
  case 18:
    if (next_state.type_ == SMType::NUL) {
      token_ptr_->get<WriteToken>()->appendData(next_state.data_);
      state_ = 18;
      return true;
    }
    break;
  }
  reset();
  return false;
}

void sdb::StateMachine::reset() {
  state_ = 0;
  clear();
}