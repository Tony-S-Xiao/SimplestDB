#include"simplestdb_token_sqlquery.h"


void sdb::QueryToken::setCondition(std::string condition) {
		condition_ = condition;
}
std::string sdb::QueryToken::getCondition()
{
		return condition_;
}