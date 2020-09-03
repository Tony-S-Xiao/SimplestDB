#pragma once
#include<string>
#include<unordered_map>
#include"token.h"

using namespace std;

class Parser
{

public:
	~Parser();
	Parser();
	Token* initToken(string);

private:
	Token* token;

};

