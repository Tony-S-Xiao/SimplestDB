#include"simplestdb_token_sql.h"

#include<string>
#include<vector>

sdb::SQLToken::SQLToken() : Token() {};
void sdb::SQLToken::setTableName(std::string name) {
		table_name_ = name;
}
std::string sdb::SQLToken::getTableName() {
		return table_name_;
}
void sdb::SQLToken::pushBackColumnName(std::string _name) {
		column_names_.push_back(_name);
		return;
}
void sdb::SQLToken::pushBackColumnType(SQLType _type) {
		column_types_.push_back(_type);
		return;
}
std::vector<std::string> sdb::SQLToken::getColumnNames() {
		return column_names_;
}
std::vector<sdb::SQLType> sdb::SQLToken::getColumnTypes() {
		return column_types_;
}
