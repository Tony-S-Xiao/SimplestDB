
sdb::Token::Token()
{

}

sdb::Token::Token(Command inputCommand, std::string data)
{
	Token::type = inputCommand;
	Token::data = data;
}

sdb::Token::~Token()
{

}

std::string sdb::Token::getText()
{
	return Token::data;
}