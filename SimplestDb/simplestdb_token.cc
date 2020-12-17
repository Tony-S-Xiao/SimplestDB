#include"simplestdb_token.h"

#include<string>
#include<vector>

sdb::Token::Token() {}
void sdb::Token::setWellFormedFlag(bool state) {
		well_formed_ = state;
	return;
}
bool sdb::Token::getWellFormedFlag() {
	return well_formed_;
}
void sdb::Token::setTokenType(sdb::Operation token_type) {
		operation_ = token_type;
}
sdb::Operation sdb::Token::getTokenType() {
		return operation_;
}