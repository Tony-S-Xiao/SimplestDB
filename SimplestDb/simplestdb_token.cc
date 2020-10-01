#include"simplestdb_token.h"

sdb::Token::Token() {}
sdb::Token::~Token() {}
void sdb::Token::setTokenType(std::string _type) {
	token_type = _type;
}
std::string sdb::Token::getTokenType() {
	return token_type;
}
sdb::MetaToken::MetaToken() {}
sdb::MetaToken::~MetaToken() {}
void sdb::MetaToken::appendData(std::string _data) {
	data.push_back(_data);
	return;
}
void sdb::MetaToken::appendCommand(std::string _command) {
	command.push_back(_command);
	return;
}
std::vector<std::string>* sdb::MetaToken::getData() {
	return &data;
}
std::vector<std::string>* sdb::MetaToken::getCommand() {
	return &command;
}

void sdb::SQLToken::setName(std::string _name) {
	table_name = _name;
}
std::string sdb::SQLToken::getName() {
	return table_name;
}
void sdb::SQLToken::pushBackColumnName(std::string _name) {
	column_names.push_back(_name);
	return;
}
void sdb::SQLToken::pushBackColumnType(SQLType _type) {
	column_types.push_back(_type);
	return;
}
std::vector<std::string>* sdb::SQLToken::getColumnNames() {
	return &column_names;
}
std::vector<sdb::SQLType>* sdb::SQLToken::getColumnTypes() {
	return &column_types;
}
sdb::SQLToken::SQLToken() {}
sdb::SQLToken::~SQLToken() {}

sdb::WriteToken::WriteToken() {}
sdb::WriteToken::~WriteToken() {}
void sdb::WriteToken::appendData(std::string _data) {
	data.push_back(_data);
	return;
}
std::vector<std::string>* sdb::WriteToken::getData()
{
	return &data;
}

sdb::QueryToken::QueryToken() {}
sdb::QueryToken::~QueryToken() {}
void sdb::QueryToken::setCondition(std::string _condition) {
	condition = _condition;
}
std::string sdb::QueryToken::getCondition()
{
	return condition;
}

sdb::CreateTableToken::CreateTableToken() {}
sdb::CreateTableToken::~CreateTableToken() {}
