#pragma once
#include<string>


using namespace std;

enum class Command { INSERT, EXIT, NONE, QUERY, CONFIG };

class Token
{
public:
	Token();
	Token(Command, string);
	~Token();
	string getText();

private:
	Command type;
	string data;

};
