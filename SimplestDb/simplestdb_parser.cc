#include"simplestdb_parser.h"

#include<algorithm>
#include<string>
#include<sstream>
#include<iostream>
#include<vector>

sdb::Parser::~Parser() {}

sdb::Parser::Parser() {}

sdb::Token* sdb::Parser::createToken(std::vector<std::string>* input_vector) {
	if (input_vector->size() == 0) {
		delete input_vector;
		return nullptr;
	}
	Token* result{ nullptr };
	std::vector<std::string> keywords;
	std::vector<std::vector<std::string>> data{};

	//meta token creation
	if ((*input_vector)[0][0] == '.') {
		std::string command_word = (*input_vector)[0].substr(1);
		std::transform(command_word.begin(), command_word.end(), command_word.begin(), [](char a) { return std::tolower(a); });
		result = new MetaToken();

		if (command_word == std::string("open")) {
			result->setTokenType(TokenType::OPEN);
			if (input_vector->size() > 1) {
				result->appendData((*input_vector)[1]);
				result->setWellFormedFlag(true);
			}
		}
		else if (command_word == std::string("cd")) {
			result->setTokenType(TokenType::CD);
			if (input_vector->size() > 1) {
				result->appendData((*input_vector)[1]);
				result->setWellFormedFlag(true);
			}
		}
		else if (command_word == std::string("help")) {
			result->setTokenType(TokenType::HELP);
			result->setWellFormedFlag(true);
		}
		else if (command_word == std::string("create")) {
			result->setTokenType(TokenType::CREATE);
			if (input_vector->size() > 1) {
				result->appendData((*input_vector)[1]);
				result->setWellFormedFlag(true);
			}
		}
	}

	//SQL token creation
	else {
		//separate input into keywords and data(separated)
		int curr_index = 0;
		while (curr_index < input_vector->size()) {
			std::string lowercase_word = (*input_vector)[curr_index];
			std::transform(lowercase_word.begin(), lowercase_word.end(), lowercase_word.begin(), [](char a) { return std::tolower(a); });

			while (SQLCommandSet.find(lowercase_word) != SQLCommandSet.end() && curr_index < input_vector->size()) {
				keywords.push_back(lowercase_word);
				++curr_index;
				if (curr_index < input_vector->size()) {
					lowercase_word = (*input_vector)[curr_index];
					std::transform(lowercase_word.begin(), lowercase_word.end(), lowercase_word.begin(), [](char a) { return std::tolower(a); });
				}
			}
			data.push_back({});
			while (SQLCommandSet.find(lowercase_word) == SQLCommandSet.end() && curr_index < input_vector->size()) {
				std::string data_word = (*input_vector)[curr_index];
				
				if (data_word.front() == ',' || data_word.front() == '(') {
					data_word = data_word.substr(1);
				}
				if (data_word.back() == ',' || data_word.back() == ')') {
					data_word.pop_back();
				}
				if(data_word.size() > 0)
				data[data.size()-1].push_back(data_word);
				++curr_index;
				if (curr_index < input_vector->size()) {
					lowercase_word = (*input_vector)[curr_index];
					std::transform(lowercase_word.begin(), lowercase_word.end(), lowercase_word.begin(), [](char a) { return std::tolower(a); });
				}
			}
		}
	//create token
		if (keywords.size() > 0 && keywords[0] == std::string("select")) {
			std::cout << "select" << std::endl;


		}
		else if (keywords.size() > 1 && keywords[0] == std::string("insert") && keywords[1] == std::string("into")) {
			std::cout << "insert" << std::endl;
		}
		else if (keywords.size() > 1 && keywords[0] == std::string("create") && keywords[1] == std::string("table")) {
			std::cout << "create" << std::endl;
			result = new CreateTableToken();
			result->setName(data[0][0]);
			for (int i = 1; i < data[0].size()-1; i+=2) {
				result->pushBackColumnName(data[0][i]);
				SQLType type = SQLType::NUL;
				
			}
		}
	}
	//if (result == nullptr || !result->getWellFormedFlag()) {
	//	std::cout << "Invalid command." << std::endl;
	//	delete result;
	//	result = nullptr;
	//}
	delete input_vector;
	return result;
}

//sdb::SQLType convertSQLType(std::string input) {
//	
//}

std::vector<std::string>* sdb::Parser::getCommand() {
	std::string curr_line{ "" };
	std::getline(std::cin, curr_line);
	std::istringstream iss(curr_line);

	std::vector<std::string>* input = new std::vector<std::string>();
	std::string curr_word = "";

	while (iss >> curr_word) {
		input->push_back(curr_word);
	}
	std::cin.clear();
	return input;
}
