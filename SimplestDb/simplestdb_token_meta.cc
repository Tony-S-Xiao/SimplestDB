#include"simplestdb_token_meta.h"

#include<string>
void sdb::MetaToken::setData(std::string data) {
		data_ = data;
}
std::string sdb::MetaToken::getData() {
		return data_;
}