#include"simplestdb_prompter.h"

#include<string>
#include<fstream>
#include<iostream>

sdb::Prompter::Prompter() {

}
sdb::Prompter::~Prompter() {

}
void sdb::Prompter::load(std::string filename) {
	std::ifstream file(filename);

	while (file.good()) {
		std::string temp{ "" };
		std::getline(file, temp);
		std::string first_word{ temp.substr(0, temp.find_first_of(' ') + 1) };
		std::string prompt{ temp.substr(temp.find_first_of(' ') + 1) };
		data.insert({ static_cast<int>(convertToCommand(first_word)), prompt });
	}
}
sdb::PrompterCommand sdb::Prompter::convertToCommand(std::string data) {
	if (data == std::string("help:")) {
		return PrompterCommand::HELP;
	}
	else if (data == std::string("unimplemented_command:")) {
		return PrompterCommand::UNIMPLEMENTED;
	}
	else if (data == std::string("invalid_command:")) {
		return PrompterCommand::INVALID;
	}
	return PrompterCommand::NUL;
}
void sdb::Prompter::print(PrompterCommand prompt) {
	std::cout << data.find(static_cast<int>(prompt))->second << std::endl;
}
void sdb::Prompter::print(std::string prompt) {
	std::cout << prompt << std::endl;
}