#include"simplestdb_token.h"
#include"simplestdb_parser.h"

#include<string>
#include<iostream>
#include<vector>

sdb::Parser::~Parser() {}

sdb::Parser::Parser() {}

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
		std::cout << *(--parsed.end()) << std::endl;
	}
	
	if (first_word == std::string("create")) {
		output_token = new CreateTableToken();
		output_token->setTokenType("create");
		output_token->setName(parsed[2]);
	} else if (first_word == std::string("select")) {
		output_token = new MetaToken();
		output_token->setTokenType("select");

	} else if (first_word == std::string("insert")) {
		output_token = new CreateTableToken();
		output_token->setTokenType("insert");

	} else {
		std::cout << "Invalid SQL command." << std::endl;
	}
	return output_token;
}

