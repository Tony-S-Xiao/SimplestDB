#pragma once
#include"simplestdb_table.h"

#include<string>

sdb::Table::Table(std::string& name, std::vector<SQLTypes>& header) : name(name) {
	Table::header = header;
}
sdb::Table::~Table() {

}
std::string sdb::Table::getName() const {
	return name;
}
void sdb::Table::insertRow(uint32_t row_id, std::string data) {

}
void sdb::Table::setFieldByID(uint32_t row_id, std::string column_name) {

}
std::string sdb::Table::getFieldByID(uint32_t row_id, std::string column_name) const {

}