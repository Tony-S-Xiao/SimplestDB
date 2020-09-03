#include"token.h"

Token::Token()
{

}

Token::~Token()
{

}

Token::Token(Command inputCommand, string data)
{
	Token::type = inputCommand;
	Token::data = data;
}

string Token::getText()
{
	return Token::data;
}