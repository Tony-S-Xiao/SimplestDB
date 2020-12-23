#include"simplestdb_token.h"
#include"simplestdb_token_meta.h"
#include"simplestdb_token_sqlcreate.h"
#include"simplestdb_token_sqlquery.h"
#include"simplestdb_token_sqlwritetoken.h"

#include<string>
#include<vector>
#include<memory>

sdb::Token::Token(std::unique_ptr<MetaToken>&& token, Operation operation) :
		meta_token_ptr_{ std::move(token) },
		operation_(operation)
{}
sdb::Token::Token(std::unique_ptr<CreateTableToken>&& token, Operation operation) :
		create_table_token_ptr_{ std::move(token) },
		operation_(operation)
{}
sdb::Token::Token(std::unique_ptr<QueryToken>&& token, Operation operation) :
		query_token_ptr_{ std::move(token) },
		operation_(operation)
{}
sdb::Token::Token(std::unique_ptr<WriteToken>&& token, Operation operation) :
		write_token_ptr_{ std::move(token) },
		operation_(operation)
{}
sdb::Token::Token(MetaToken* token, Operation operation) :
		meta_token_ptr_(token),
		operation_(operation)
{}
sdb::Token::Token(CreateTableToken* token, Operation operation) :
		create_table_token_ptr_(token),
		operation_(operation)
{}
sdb::Token::Token(QueryToken* token, Operation operation) :
		query_token_ptr_(token),
		operation_(operation)
{}
sdb::Token::Token(WriteToken* token, Operation operation) :
		write_token_ptr_(token),
		operation_(operation)
{}
sdb::Token::Token() {}
template<class C>
C* sdb::Token::get() {
		return nullptr;
}
template<>
sdb::MetaToken* sdb::Token::get<sdb::MetaToken>() {
		return &*meta_token_ptr_;
}
template<>
sdb::CreateTableToken* sdb::Token::get<sdb::CreateTableToken>() {
		return &*create_table_token_ptr_;
}
template<>
sdb::QueryToken* sdb::Token::get<sdb::QueryToken>() {
		return &*query_token_ptr_;
}
template<>
sdb::WriteToken* sdb::Token::get<sdb::WriteToken>() {
		return &*write_token_ptr_;
}
void sdb::Token::setWellFormedFlag(bool state) {
	well_formed_ = state;
	return;
}
bool sdb::Token::getWellFormedFlag() {
	return well_formed_;
}
void sdb::Token::setOperationType(sdb::Operation token_type) {
		operation_ = token_type;
}
sdb::Operation sdb::Token::getTokenType() {
		return operation_;
}
