#include"simplestdb_token_sqlwritetoken.h"

#include<string>
#include<vector>
void sdb::WriteToken::appendData(std::string data) {
		data_.push_back(data);
		return;
}
std::vector<std::string> sdb::WriteToken::getData()
{
		return data_;
}
