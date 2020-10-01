
#include"simplestdb_token.h"
#include"simplestdb_parser.h"

#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<fstream>

int main()
{
	sdb::WriteToken wow;
	sdb::Parser parser_wow;
	std::string input;
	while (true) {
		std::cout << "enter command" << std::endl;
		getline(std::cin, input); //NOTE: cin automatically separate by whitespace

		sdb::Token* curr_token = parser_wow.createToken(input);
		delete curr_token;
	}
	

	return 0;
}



