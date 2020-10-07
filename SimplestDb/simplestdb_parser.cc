#include"simplestdb_parser.h"

#include<string>
#include<iostream>
#include<vector>

sdb::Parser::~Parser() {}

sdb::Parser::Parser() {}
/*
1.separate into keywords and non-keywords arrays
2.create token
3.fill token and convert to useable data
*/
sdb::Token* sdb::Parser::createToken(std::string input)
{
	//find the type of command
	int first_space = input.find_first_of(" ");
	std::string first_word = input.substr(0, first_space);
	Token* output_token = nullptr;
	//parse the string: 
	//get rid of all delimiters(separate words)
	//to lower case all letters
	std::vector<std::string> parsed{ "" };
	for (int i = 0; i < input.size(); ++i) {
		//convert to lower case
		if (input[i] <= 'z' && input[i] >= 'a' ||
			input[i] <= 'Z' && input[i] >= 'A')
			parsed[parsed.size() - 1].push_back(std::tolower(input[i]));
		//start new word
		else if ((--parsed.end())->size() != 0) {
			parsed.push_back("");
		}
		//TODELETE: std::cout << *(--parsed.end()) << std::endl;
	}
	//metatoken creation
	if (first_word[0] == '.') {
		output_token = new MetaToken();
		first_word = first_word.substr(1);
		if (first_word == std::string("open")) {
			output_token->setTokenType(TokenType::OPEN);
			output_token->appendData(parsed[1]);
		} else if (first_word == std::string("cd")) {
			output_token->setTokenType(TokenType::CD);
			output_token->appendData(parsed[1]);
		} else if (first_word == std::string("help")) {
			output_token->setTokenType(TokenType::HELP);
		} else if (first_word == std::string("create")) {
			output_token->setTokenType(TokenType::CREATE);
			output_token->appendData(parsed[1]); // TODO: ".create" causes crash since there is no other words
		}
	}
	//sqltoken creation
	else if (first_word == std::string("create")) {
		output_token = new CreateTableToken();
		output_token->setTokenType(TokenType::NEW);

	} else if (first_word == std::string("select")) {
		output_token = new QueryToken();
		output_token->setTokenType(TokenType::READ);

	} else if (first_word == std::string("insert")) {
		output_token = new WriteToken();
		output_token->setTokenType(TokenType::WRITE);

	} else {
	std::cout << "Invalid SQL command." << std::endl;
	}
		
	
	return output_token;
}

