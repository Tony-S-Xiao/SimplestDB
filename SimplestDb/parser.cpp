#include"parser.h"
#include"token.h"
#include<string>
#include<iostream>

using namespace std;

Parser::Parser()
{

}

Parser::~Parser()
{

}

Token* Parser::initToken(string userInput)
{
	if(userInput.length() == 0) return nullptr;

	if (userInput[0] == '.')
	{
		string s = userInput.substr(1, userInput.find_first_of(' ')-1);
		
		if (s == string("config"))
		{
			return new Token(Command::CONFIG, userInput.substr(userInput.find_first_of(' ') + 1));
		}
	}
	
	return nullptr;
}
