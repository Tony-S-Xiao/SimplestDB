#pragma once
#include<string>


using namespace std;

enum class Command { INSERT, EXIT, NONE, QUERY };

class Token
{
public:
	Token();
	Token(Command, string);
	~Token();

private:
	Command type;
	string data;

};