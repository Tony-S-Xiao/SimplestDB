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


private:
	Token* token;

};

