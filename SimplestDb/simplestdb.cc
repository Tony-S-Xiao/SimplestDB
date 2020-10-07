#include"simplestdb_token.h"
#include"simplestdb_parser.h"

#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<fstream>

int main()
{
	sdb::Parser parser_wow;
	std::string input;
	while (true) {
		std::cout << "enter command:" << std::endl;
		getline(std::cin, input); //NOTE: cin automatically separate by whitespace

		sdb::Token* curr_token = parser_wow.createToken(input);
		switch (curr_token->getTokenType()) {
		case sdb::TokenType::OPEN:
			std::cout << "open" << std::endl;
			break;
		case sdb::TokenType::CD:
			std::cout << "cd" << std::endl;
			break;
		case sdb::TokenType::HELP:
			std::cout << "help" << std::endl;
			break;
		case sdb::TokenType::CREATE:
			std::cout << "create" << std::endl;
			break;
		default: 
			std::cout << "wow" << std::endl;
		}
		delete curr_token;
	}
	

	return 0;
}



	